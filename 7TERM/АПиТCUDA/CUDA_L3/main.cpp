#include <chrono>
#include <cstddef>
#include <cuda_runtime.h>
#include <curand.h>
#include <iomanip>
#include <iostream>

#define COLOR_1 "\x1b[38;2;100;150;255m"
#define COLOR_2 "\x1b[38;2;100;255;150m"
#define COLOR_3 "\x1b[38;2;255;255;100m"
#define COLOR_4 "\x1b[38;2;255;150;100m"
#define RESET   "\x1b[0m"

using namespace std;

void cudaWindowTransform(unsigned int* d_src, unsigned int* d_dst, size_t N, size_t M);

const unsigned long long SEED = 1234ULL;

template <typename T>
void windowTransform(T* src, T* dst, size_t N, size_t M) {
    auto start = chrono::high_resolution_clock::now();
    size_t out_rows = N / 4;
    size_t out_cols = M * 4;
    for (size_t j = 0; j < M; ++j) {
        for (size_t i = 0; i < out_rows; ++i) {
            size_t base_y = i * 4;
            T v1 = src[(base_y + 0) * M + j];
            T v2 = src[(base_y + 1) * M + j];
            T v3 = src[(base_y + 2) * M + j];
            T v4 = src[(base_y + 3) * M + j];
            size_t out_idx = i * out_cols + j * 4;
            dst[out_idx + 0] = v2;
            dst[out_idx + 1] = v4;
            dst[out_idx + 2] = v3;
            dst[out_idx + 3] = v1;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "Time taken (CPU): " << fixed << setprecision(3) << ms << " ms\n";
}

template <typename T>
void print_matrix_colored(T* mat, size_t N, size_t M, bool is_source = true) {
    const size_t MAX_ELEMENTS = 192;
    const size_t MAX_PER_LINE = 16;

    if (N * M > MAX_ELEMENTS) {
        cout << "Too big (" << N << "x" << M << " = " << N * M << " elements).\n";
        return;
    }

    const char* colors[4] = { COLOR_1, COLOR_2, COLOR_3, COLOR_4 };

    for (size_t i = 0; i < N; ++i) {
        size_t count = 0;
        for (size_t j = 0; j < M; ++j) {
            int src_pos;

            if (is_source) {
                src_pos = i % 4;   
            }
            else {
                int out_pos = j % 4;   
                src_pos = (out_pos == 0) ? 1 :  
                    (out_pos == 1) ? 3 :   
                    (out_pos == 2) ? 2 : 0;  
            }

            cout << colors[src_pos] << setw(4) << mat[i * M + j] << " " << RESET;
            if (++count % MAX_PER_LINE == 0 && j != M - 1) {
                cout << "\n    ";
            }
        }
        cout << "\n";
    }
}

template <typename T>
bool compare_matrices(T* a, T* b, size_t N, size_t M) {
    for (size_t i = 0; i < N * M; ++i)
        if (a[i] != b[i]) return false;
    return true;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <N> <M>\n";
        return 1;
    }

    size_t N = atoi(argv[1]);
    size_t M = atoi(argv[2]);
    if (N % 4 != 0) {
        cerr << "N must be divisible by 4\n";
        return 1;
    }

    size_t out_rows = N / 4;
    size_t out_cols = M * 4;
    size_t out_size = out_rows * out_cols;

    unsigned int* h_src = new unsigned int[N * M];
    unsigned int* h_dst = new unsigned int[out_size];
    unsigned int* h_ref = new unsigned int[out_size];
    unsigned int* d_src, * d_dst;

    cudaMalloc(&d_src, N * M * sizeof(unsigned int));
    cudaMalloc(&d_dst, out_size * sizeof(unsigned int));

    curandGenerator_t gen;
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
    curandSetPseudoRandomGeneratorSeed(gen, SEED);
    curandGenerate(gen, d_src, N * M);
    cudaMemcpy(h_src, d_src, N * M * sizeof(unsigned int), cudaMemcpyDeviceToHost);

    for (size_t i = 0; i < N * M; ++i) {
        h_src[i] = (h_src[i] % 900) + 100;
    }
    cudaMemcpy(d_src, h_src, N * M * sizeof(unsigned int), cudaMemcpyHostToDevice);

    cout << "Source Matrix (" << N << "x" << M << "):\n";
    print_matrix_colored(h_src, N, M, true);
    cout << "------------------------\n";

    windowTransform(h_src, h_ref, N, M);
    cout << "CPU Result (" << out_rows << "x" << out_cols << "):\n";
    print_matrix_colored(h_ref, out_rows, out_cols, false);
    cout << "------------------------\n";

    cudaWindowTransform(d_src, d_dst, N, M);
    cudaMemcpy(h_dst, d_dst, out_size * sizeof(unsigned int), cudaMemcpyDeviceToHost);
    cout << "GPU Result (" << out_rows << "x" << out_cols << "):\n";
    print_matrix_colored(h_dst, out_rows, out_cols, false);

    if (compare_matrices(h_ref, h_dst, out_rows, out_cols)) {
        cout << "Results match\n";
    }
    else {
        cout << "Mismatch\n";
    }

    curandDestroyGenerator(gen);
    cudaFree(d_src); cudaFree(d_dst);
    delete[] h_src; delete[] h_dst; delete[] h_ref;
    return 0;
}