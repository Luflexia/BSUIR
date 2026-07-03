#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdio>

const size_t BLOCK_SIZE_X     = 16;
const size_t BLOCK_SIZE_Y     = 16;
const size_t BLOCK_SIZE_SCAN  = 256;

__global__ void windowTransformKernel(unsigned int* d_src, unsigned int* d_dst, size_t N, size_t M) {
    int out_y = blockIdx.y * blockDim.y + threadIdx.y;
    int j     = blockIdx.x * blockDim.x + threadIdx.x;

    size_t out_rows = N / 4;
    size_t out_cols = M * 4;

    if (out_y >= out_rows || j >= M) return;

    size_t base_y = out_y * 4;
    unsigned int v1 = d_src[(base_y + 0) * M + j];
    unsigned int v2 = d_src[(base_y + 1) * M + j];
    unsigned int v3 = d_src[(base_y + 2) * M + j];
    unsigned int v4 = d_src[(base_y + 3) * M + j];

    size_t out_idx = out_y * out_cols + j * 4;
    d_dst[out_idx + 0] = v2;
    d_dst[out_idx + 1] = v4;
    d_dst[out_idx + 2] = v3;
    d_dst[out_idx + 3] = v1;
}

void cudaWindowTransform(unsigned int* d_src, unsigned int* d_dst, size_t N, size_t M) {
    size_t out_rows = N / 4;
    dim3 block(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid((M + block.x - 1) / block.x, (out_rows + block.y - 1) / block.y);

    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);
    cudaEventRecord(start);

    windowTransformKernel<<<grid, block>>>(d_src, d_dst, N, M);

    cudaGetLastError();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms = 0.0f;
    cudaEventElapsedTime(&ms, start, stop);
    printf("Time taken (GPU): %.3f ms\n", ms);

    cudaEventDestroy(start); cudaEventDestroy(stop);
}

// Один блок сканирует участок длиной sectionSize (<= BLOCK_SIZE_SCAN)
__global__ void koggeStoneScanKernel(const unsigned int* d_in, unsigned int* d_out, unsigned int* d_block_sums, size_t total, size_t sectionSize)
{
    extern __shared__ unsigned int sdata[];

    const size_t gid = blockIdx.x * sectionSize + threadIdx.x;
    const unsigned int tid = threadIdx.x;

    // загрузка в shared
    if (tid < sectionSize && gid < total)
        sdata[tid] = d_in[gid];
    else
        sdata[tid] = 0;
    __syncthreads();

    // инклюзивный Kogge–Stone в пределах блока
    for (unsigned int offset = 1; offset < sectionSize; offset <<= 1) {
        unsigned int val = 0;
        if (tid >= offset && tid < sectionSize)
            val = sdata[tid - offset];
        __syncthreads();
        if (tid >= offset && tid < sectionSize)
            sdata[tid] += val;
        __syncthreads();
    }

    // запись результата
    if (tid < sectionSize && gid < total)
        d_out[gid] = sdata[tid];

    // запись суммы блока
    if (d_block_sums != nullptr && tid == sectionSize - 1) {
        d_block_sums[blockIdx.x] = sdata[tid];
    }
}

__global__ void addBlockSums(unsigned int* d_out, const unsigned int* d_block_sums, size_t total, size_t sectionSize) {
    size_t gid = blockIdx.x * sectionSize + threadIdx.x;
    if (gid >= total) return;
    
    if (blockIdx.x > 0) {
        d_out[gid] += d_block_sums[blockIdx.x - 1];
    }
}

void cudaPrefixSumImpl(unsigned int* d_src, unsigned int* d_dst, size_t total) {
    const size_t sectionSize = BLOCK_SIZE_SCAN;
    size_t numBlocks = (total + sectionSize - 1) / sectionSize;

    dim3 block(sectionSize);
    dim3 grid(numBlocks);

    size_t shmemBytes = sectionSize * sizeof(unsigned int);

    unsigned int* d_block_sums = nullptr;
    if (numBlocks > 1) {
        cudaMalloc(&d_block_sums, numBlocks * sizeof(unsigned int));
    }

    // 1. Scan within each block and collect block sums
    koggeStoneScanKernel<<<grid, block, shmemBytes>>>(d_src, d_dst, d_block_sums, total, sectionSize);
    
    if (numBlocks > 1) {
        // 2. Recursively scan the block sums
        cudaPrefixSumImpl(d_block_sums, d_block_sums, numBlocks);

        // 3. Add block sums to the results
        addBlockSums<<<grid, block>>>(d_dst, d_block_sums, total, sectionSize);
        
        cudaFree(d_block_sums);
    }
}

// Обёртка, которую вызывает main4.cpp
void cudaPrefixSum(unsigned int* d_src, unsigned int* d_dst, size_t total) {
    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);
    cudaEventRecord(start);

    cudaPrefixSumImpl(d_src, d_dst, total);
    cudaDeviceSynchronize();
    cudaGetLastError();

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float ms = 0.0f;
    cudaEventElapsedTime(&ms, start, stop);
    printf("Time taken (GPU prefix sum): %.3f ms\n", ms);

    cudaEventDestroy(start); cudaEventDestroy(stop);
}

