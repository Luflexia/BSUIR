#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdio>

const size_t BLOCK_SIZE_X = 16;
const size_t BLOCK_SIZE_Y = 16;

__global__ void windowTransformKernel(unsigned int* d_src, unsigned int* d_dst, size_t N, size_t M) {
    int out_y = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;

    size_t out_rows = N / 4;
    size_t out_cols = M * 4;

    if (out_y >= out_rows || j >= M) return;

    size_t base_y = out_y * 4;
    unsigned int v1 = d_src[(base_y + 0) * M + j];
    unsigned int v2 = d_src[(base_y + 1) * M + j];
    unsigned int v3 = d_src[(base_y + 2) * M + j];
    unsigned int v4 = d_src[(base_y + 3) * M + j];

    size_t out_idx = out_y * out_cols + j * 4;
    d_dst[out_idx + 0] = v2;  // 2
    d_dst[out_idx + 1] = v4;  // 4
    d_dst[out_idx + 2] = v3;  // 3
    d_dst[out_idx + 3] = v1;  // 1
}

void cudaWindowTransform(unsigned int* d_src, unsigned int* d_dst, size_t N, size_t M) {
    size_t out_rows = N / 4;

    dim3 block(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid((M + block.x - 1) / block.x, (out_rows + block.y - 1) / block.y);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    windowTransformKernel << <grid, block >> > (d_src, d_dst, N, M);

    cudaGetLastError();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms = 0.0f;
    cudaEventElapsedTime(&ms, start, stop);
    printf("GPU elapsed: %.3f ms\n", ms);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
}