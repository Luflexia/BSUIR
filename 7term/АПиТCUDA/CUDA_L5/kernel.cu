#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdio>
#include <algorithm>

const int BLOCK_SIZE_X = 16;
const int BLOCK_SIZE_Y = 16;

__global__ void splitChannelsKernel(const unsigned char*  d_src,
    unsigned char*  d_r,
    unsigned char*  d_g,
    unsigned char*  d_b,
    int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    int idx = y * width + x;
    int src_idx = idx * 3;

    d_r[idx] = d_src[src_idx + 0];
    d_g[idx] = d_src[src_idx + 1];
    d_b[idx] = d_src[src_idx + 2];
}

__global__ void mergeChannelsKernel(unsigned char*  d_dst,
    const unsigned char*  d_r,
    const unsigned char*  d_g,
    const unsigned char*  d_b,
    int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    int idx = y * width + x;
    int dst_idx = idx * 3;

    d_dst[dst_idx + 0] = d_r[idx];
    d_dst[dst_idx + 1] = d_g[idx];
    d_dst[dst_idx + 2] = d_b[idx];
}

__global__ void laplaceH3Kernel(const unsigned char*  d_src,
    unsigned char*  d_dst,
    int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    // Фильтр Лапласа H3
    // [ 1, -2,  1]
    // [-2,  4, -2]
    // [ 1, -2,  1]

    int sum = 0;

    // Проходим по окну 3x3
    for (int ky = -1; ky <= 1; ++ky) {
        for (int kx = -1; kx <= 1; ++kx) {
            // Координаты соседа с проверкой границ (clamp)
            int nx = min(max(x + kx, 0), width - 1);
            int ny = min(max(y + ky, 0), height - 1);

            int val = d_src[ny * width + nx];
            int coeff = 0;

            // Определяем коэффициент фильтра
            if (kx == 0 && ky == 0) coeff = 4;
            else if (abs(kx) == 1 && abs(ky) == 1) coeff = 1; // углы
            else coeff = -2; // крест

            sum += val * coeff;
        }
    }

    // Клампинг результата в [0, 255]

    sum = abs(sum);
    sum = min(max(sum, 0), 255);
    d_dst[y * width + x] = (unsigned char)sum;
}

// Обертка для вызова ядер
void cudaLaplaceFilterRGB(const unsigned char* d_src, unsigned char* d_dst, int width, int height) {
    int numPixels = width * height;

    // Выделяем память под каналы
    unsigned char* d_r, * d_g, * d_b;
    unsigned char* d_r_out, * d_g_out, * d_b_out;

    cudaMalloc(&d_r, numPixels);
    cudaMalloc(&d_g, numPixels);
    cudaMalloc(&d_b, numPixels);
    cudaMalloc(&d_r_out, numPixels);
    cudaMalloc(&d_g_out, numPixels);
    cudaMalloc(&d_b_out, numPixels);

    // Конфигурация сетки
    dim3 block(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);

    // 1. Разделение каналов
    splitChannelsKernel << <grid, block >> > (d_src, d_r, d_g, d_b, width, height);
    cudaGetLastError();

    // 2. Обработка каждого канала
    laplaceH3Kernel << <grid, block >> > (d_r, d_r_out, width, height);
    laplaceH3Kernel << <grid, block >> > (d_g, d_g_out, width, height);
    laplaceH3Kernel << <grid, block >> > (d_b, d_b_out, width, height);
    cudaGetLastError();

    // 3. Сборка каналов
    mergeChannelsKernel << <grid, block >> > (d_dst, d_r_out, d_g_out, d_b_out, width, height);
    cudaGetLastError();

    // Очистка
    cudaFree(d_r); cudaFree(d_g); cudaFree(d_b);
    cudaFree(d_r_out); cudaFree(d_g_out); cudaFree(d_b_out);
}

// Обертка для Grayscale
void cudaLaplaceFilterGray(const unsigned char* d_src, unsigned char* d_dst, int width, int height) {
    dim3 block(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);

    laplaceH3Kernel << <grid, block >> > (d_src, d_dst, width, height);
    cudaGetLastError();
}