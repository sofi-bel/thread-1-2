#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>

#define NUM_THREADS 8

void sumFunction(int N, unsigned long long first, unsigned long long last, double **matrixA, double **matrixB, double **productMatrixParallel) {
    for (unsigned long long i = first; i < last; ++i) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                productMatrixParallel[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

int main() {
    std::thread threads[NUM_THREADS];
    const int N = 4096;
    unsigned long long step = N / NUM_THREADS;

    double **matrixA, **matrixB, **productMatrixParallel, **productMatrixSerial;
    matrixA = new double*[N];
    matrixB = new double*[N];
    productMatrixParallel = new double*[N];
    productMatrixSerial = new double*[N];
    srand(time(nullptr));

    for (int i = 0; i <  N; i++) {
        matrixA[i] = new double[N];
        matrixB[i] = new double[N];
        productMatrixParallel[i] = new double[N];
        productMatrixSerial[i] = new double[N];
    }

    for(int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrixA[i][j] = 1 + rand() % 100;
            matrixB[i][j] = 1 + rand() % 100;
        }
    }

    std::cout << "Start multithread calculation..." << std::endl;
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::thread(sumFunction, N, i * step, (i + 1) * step, matrixA, matrixB, productMatrixParallel);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
        std::cout << "Thread " << i << " end calculation " << std::endl;
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "End multithread calculation with time: "  << elapsed1 << " msec" << std::endl;


    std::cout << "Start single thread calculation ..." << std::endl;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                productMatrixSerial[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    end = std::chrono::system_clock::now();
    auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "End single thread calculation with time: "  << elapsed2 << " msec" << std::endl;

    std::cout << "Matrix check:" << std::endl;
    bool flag = 0;
    for(int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if(productMatrixSerial[i][j]-productMatrixParallel[i][j] != 0 ) {
                flag = 1;
                break;
            }
        }
    }

    if (flag == 1) {
        std::cout << "matrices are not equal" << " ";
    } else {
        std::cout << "matrices are equal" << " ";
    }
}
