#include <iostream>
#include <random>
#include <chrono>
#include <immintrin.h>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <omp.h>

constexpr size_t L3_CACHE_SIZE = 16 * 1024 * 1024;
constexpr size_t K = 16;
constexpr size_t N = 150;
constexpr size_t ALIGNMENT = 64;

size_t calculate_block_size(size_t l3_cache_bytes) {
    size_t max_block_bytes = (l3_cache_bytes / 3) * 9 / 10;

    size_t s = static_cast<size_t>(std::sqrt(max_block_bytes / 4));

    s = s - (s % 16);

    s = std::max<size_t>(s, 64);

    std::cout << "L3 Cache: " << l3_cache_bytes / (1024 * 1024) << " MB\n";
    std::cout << "Max block size: " << max_block_bytes / (1024.0 * 1024.0) << " MB\n";
    std::cout << "Using block dimension: " << s << "x" << s << " ("
        << (s * s * 4) / (1024.0 * 1024.0) << " MB per block)\n";
    return s;
}

void init_matrix(float* matrix, size_t rows, size_t cols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            matrix[i * cols + j] = dis(gen);
        }
    }
}

bool compare_matrices(const float* A, const float* B, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (std::abs(A[i * cols + j] - B[i * cols + j]) > 1e-3f) {
                std::cout << "Mismatch at [" << i << "," << j << "]: "
                    << A[i * cols + j] << " vs " << B[i * cols + j] << "\n";
                return false;
            }
        }
    }
    return true;
}

#pragma optimize("", off)
void matrix_multiply_scalar(const float* A, const float* B, float* C, size_t N, size_t K) {
    const size_t total_size = N * K;

    std::memset(C, 0, total_size * total_size * sizeof(float));

#pragma omp parallel for collapse(2) schedule(dynamic)
    for (int block_i = 0; block_i < static_cast<int>(N); ++block_i) {
        for (int block_j = 0; block_j < static_cast<int>(N); ++block_j) {
            for (size_t block_k = 0; block_k < N; ++block_k) {

                for (size_t i = 0; i < K; ++i) {
                    for (size_t j = 0; j < K; ++j) {
                        for (size_t k = 0; k < K; ++k) {

                            const size_t global_row = static_cast<size_t>(block_i) * K + i;
                            const size_t global_col = static_cast<size_t>(block_j) * K + j;
                            const size_t global_k = block_k * K + k;

                            C[global_row * total_size + global_col] +=
                                A[global_row * total_size + global_k] *
                                B[global_k * total_size + global_col];
                        }
                    }
                }
            }
        }
    }
}
#pragma optimize("", on)

void matrix_multiply_avx(const float* A, const float* B, float* C, size_t size) {
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < static_cast<int>(size); ++i) {
        size_t j = 0;
        for (; j + 8 <= size; j += 8) {
            __m256 sum = _mm256_setzero_ps();

            for (size_t k = 0; k < size; ++k) {
                __m256 a = _mm256_set1_ps(A[i * size + k]);
                __m256 b = _mm256_load_ps(&B[k * size + j]);
                sum = _mm256_add_ps(sum, _mm256_mul_ps(a, b));
            }

            _mm256_store_ps(&C[i * size + j], sum);
        }

        for (; j < size; ++j) {
            float sum = 0.0f;
            for (size_t k = 0; k < size; ++k) {
                sum += A[i * size + k] * B[k * size + j];
            }
            C[i * size + j] = sum;
        }
    }
}

void matrix_multiply_block(const float* A, const float* B, float* C,
    size_t size, size_t block_size) {
    std::memset(C, 0, size * size * sizeof(float));

#pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i0 = 0; i0 < static_cast<int>(size); i0 += static_cast<int>(block_size)) {
        size_t i_end = std::min(static_cast<size_t>(i0) + block_size, size);

        for (int j0 = 0; j0 < static_cast<int>(size); j0 += static_cast<int>(block_size)) {
            size_t j_end = std::min(static_cast<size_t>(j0) + block_size, size);

            for (size_t k0 = 0; k0 < size; k0 += block_size) {
                size_t k_end = std::min(k0 + block_size, size);

                for (size_t i = static_cast<size_t>(i0); i < i_end; ++i) {
                    for (size_t j = static_cast<size_t>(j0); j < j_end; j += 8) {
                        size_t remaining = j_end - j;

                        if (remaining >= 8) {
                            __m256 sum = _mm256_load_ps(&C[i * size + j]);

                            for (size_t k = k0; k < k_end; ++k) {
                                __m256 a = _mm256_set1_ps(A[i * size + k]);
                                __m256 b = _mm256_load_ps(&B[k * size + j]);
                                sum = _mm256_add_ps(sum, _mm256_mul_ps(a, b));
                            }

                            _mm256_store_ps(&C[i * size + j], sum);
                        }
                        else {
                            for (size_t jj = j; jj < j_end; ++jj) {
                                float sum = C[i * size + jj];

                                for (size_t k = k0; k < k_end; ++k) {
                                    sum += A[i * size + k] * B[k * size + jj];
                                }

                                C[i * size + jj] = sum;
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    omp_set_num_threads(omp_get_max_threads());
    std::cout << "Using " << omp_get_max_threads() << " OpenMP threads\n\n";
    const size_t total_size = N * K;
    const size_t matrix_bytes = total_size * total_size * sizeof(float);
    const size_t optimal_block_size = calculate_block_size(L3_CACHE_SIZE);

    std::cout << "Matrix dimension: " << total_size << "x" << total_size << "\n";
    std::cout << "Matrix size: " << matrix_bytes / (1024.0 * 1024.0) << " MB\n";
    std::cout << "Optimal block size: " << optimal_block_size << "x" << optimal_block_size << "\n\n";

    float* A = (float*)_mm_malloc(matrix_bytes, ALIGNMENT);
    float* B = (float*)_mm_malloc(matrix_bytes, ALIGNMENT);
    float* C1 = (float*)_mm_malloc(matrix_bytes, ALIGNMENT);
    float* C2 = (float*)_mm_malloc(matrix_bytes, ALIGNMENT);
    float* C3 = (float*)_mm_malloc(matrix_bytes, ALIGNMENT);

    if (!A || !B || !C1 || !C2 || !C3) {
        std::cerr << "Memory allocation failed!\n";
        return 1;
    }

    std::cout << "Initializing matrices...\n";
    init_matrix(A, total_size, total_size);
    init_matrix(B, total_size, total_size);

    std::cout << "Running scalar multiplication...\n";
    auto start = std::chrono::high_resolution_clock::now();
    matrix_multiply_scalar(A, B, C1, N, K);
    auto end = std::chrono::high_resolution_clock::now();
    auto scalar_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Running AVX multiplication...\n";
    start = std::chrono::high_resolution_clock::now();
    matrix_multiply_avx(A, B, C2, total_size);
    end = std::chrono::high_resolution_clock::now();
    auto avx_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Running block multiplication...\n";
    start = std::chrono::high_resolution_clock::now();
    matrix_multiply_block(A, B, C3, total_size, optimal_block_size);
    end = std::chrono::high_resolution_clock::now();
    auto block_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Verifying results...\n";
    bool avx_correct = compare_matrices(C1, C2, total_size, total_size);
    bool block_correct = compare_matrices(C1, C3, total_size, total_size);

    std::cout << "\n=== RESULTS ===\n";
    std::cout << "Execution time:\n";
    std::cout << "Scalar multiplication: " << scalar_time << " ms\n";
    std::cout << "AVX multiplication: " << avx_time << " ms\n";
    std::cout << "Block multiplication: " << block_time << " ms\n";
    std::cout << "Speedup AVX vs Scalar: " << std::fixed << std::setprecision(2)
        << static_cast<double>(scalar_time) / avx_time << "x\n";
    std::cout << "Speedup Block vs Scalar: " << std::fixed << std::setprecision(2)
        << static_cast<double>(scalar_time) / block_time << "x\n";
    std::cout << "AVX result correct: " << (avx_correct ? "Yes" : "No") << "\n";
    std::cout << "Block result correct: " << (block_correct ? "Yes" : "No") << "\n";

    _mm_free(A);
    _mm_free(B);
    _mm_free(C1);
    _mm_free(C2);
    _mm_free(C3);

    return 0;
}