#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <cuda_runtime.h>
#include "helper_image.h"
#include "exception.h"

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define RESET "\x1b[0m"

using namespace std;

void cudaLaplaceFilterRGB(const unsigned char* d_src, unsigned char* d_dst, int width, int height);
void cudaLaplaceFilterGray(const unsigned char* d_src, unsigned char* d_dst, int width, int height);

void cpuLaplaceFilter(const unsigned char* src, unsigned char* dst, int width, int height, int channels) {
    // H3 ядро
    //  1 -2  1
    // -2  4 -2
    //  1 -2  1

    // Для каждого пикселя
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;

                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {

                        int nx = min(max(x + kx, 0), width - 1);
                        int ny = min(max(y + ky, 0), height - 1);

                        int val = src[(ny * width + nx) * channels + c];
                        int coeff = 0;

                        if (kx == 0 && ky == 0) coeff = 4;
                        else if (abs(kx) == 1 && abs(ky) == 1) coeff = 1;
                        else coeff = -2;

                        sum += val * coeff;
                    }
                }

                sum = abs(sum);
                sum = min(max(sum, 0), 255);
                dst[(y * width + x) * channels + c] = (unsigned char)sum;
            }
        }
    }
}

bool compareImages(const unsigned char* ref, const unsigned char* res, int size) {
    int diff_count = 0;
    for (int i = 0; i < size; ++i) {
        int diff = abs((int)ref[i] - (int)res[i]);
        if (diff > 1) {
            if (diff_count < 10) {
                cout << "Mismatch at " << i << ": CPU=" << (int)ref[i] << " GPU=" << (int)res[i] << endl;
            }
            diff_count++;
        }
    }
    if (diff_count > 0) {
        cout << "Total mismatches (>1): " << diff_count << endl;
        return false;
    }
    return true;
}

void processImage(const string& inputFile, const string& outputFile) {
    cout << "Processing " << inputFile << "...\n";

    unsigned char* h_src = nullptr;
    unsigned int width, height, channels;


    if (!__loadPPM(inputFile.c_str(), &h_src, &width, &height, &channels)) {
        cerr << "Failed to load image: " << inputFile << endl;
        return;
    }

    cout << "Image loaded: " << width << "x" << height << " channels: " << channels << endl;
    size_t imgSize = width * height * channels * sizeof(unsigned char);

    unsigned char* h_dst_cpu = (unsigned char*)malloc(imgSize);
    unsigned char* h_dst_gpu = (unsigned char*)malloc(imgSize);

    // CPU
    auto start_cpu = chrono::high_resolution_clock::now();
    cpuLaplaceFilter(h_src, h_dst_cpu, width, height, channels);
    auto end_cpu = chrono::high_resolution_clock::now();
    double cpu_ms = chrono::duration<double, milli>(end_cpu - start_cpu).count();
    cout << "Time taken (CPU): " << fixed << setprecision(3) << cpu_ms << " ms\n";

    // GPU 
    unsigned char* d_src, * d_dst;
    cudaMalloc(&d_src, imgSize);
    cudaMalloc(&d_dst, imgSize);

    cudaMemcpy(d_src, h_src, imgSize, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    if (channels == 1) {
        cudaLaplaceFilterGray(d_src, d_dst, width, height);
    }
    else if (channels == 3) {
        cudaLaplaceFilterRGB(d_src, d_dst, width, height);
    }
    else {
        cerr << "Unsupported number of channels: " << channels << endl;
    }

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float gpu_ms = 0;
    cudaEventElapsedTime(&gpu_ms, start, stop);
    cout << "Time taken (GPU): " << fixed << setprecision(3) << gpu_ms << " ms\n";

    cudaMemcpy(h_dst_gpu, d_dst, imgSize, cudaMemcpyDeviceToHost);

    if (compareImages(h_dst_cpu, h_dst_gpu, width * height * channels)) {
        cout << COLOR_GREEN << "Results match!" << RESET << endl;
    }
    else {
        cout << COLOR_RED << "Results mismatch!" << RESET << endl;
    }

    string cpuOutputFile = outputFile;
    string gpuOutputFile = outputFile;

    size_t extPos = cpuOutputFile.find_last_of(".");
    if (extPos != string::npos) {
        cpuOutputFile.insert(extPos, "_cpu");
        gpuOutputFile.insert(extPos, "_gpu");
    }
    else {
        cpuOutputFile += "_cpu";
        gpuOutputFile += "_gpu";
    }

    __savePPM(cpuOutputFile.c_str(), h_dst_cpu, width, height, channels);
    cout << "Saved CPU result to " << cpuOutputFile << "\n";

    __savePPM(gpuOutputFile.c_str(), h_dst_gpu, width, height, channels);
    cout << "Saved GPU result to " << gpuOutputFile << "\n";
    cout << "--------------------------------------------------\n";

    free(h_src);
    free(h_dst_cpu);
    free(h_dst_gpu);
    cudaFree(d_src);
    cudaFree(d_dst);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
}

int main() {

    system("mkdir out 2>nul");
    processImage("in/RGB.ppm", "out/RGB.ppm");
    processImage("in/GRAY.pgm", "out/GRAY.pgm");

    return 0;
}