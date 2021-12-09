#include <ctime>
#include <iostream>
#include "cblas.h"

#define N 2048
#define PRINT_N 4

void PrintMat(float A[N * N]) {
    for (int i = 0; i < PRINT_N; ++i) {
        for (int j = 0; j < PRINT_N; ++j) {
            std::cout << A[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void InverseMatrix(float A[N * N], float reversedMatrix[N * N]) {
    float maxColSum = 0, maxRowSum = 0;
    float B[N * N] = {0};
    float R[N * N] = {0};
    float Rtmp[N * N] = {0};
    float Rmul[N * N] = {0};
    float matrixSum[N * N] = {0};
    int i;

    float vector[N] = {0};
    float colSum[N] = {0};

    std::fill(vector, vector + N, 1);
    cblas_sgemv(CblasRowMajor, CblasNoTrans, N, N,
                1, A, N, vector, 1, 0, colSum, 1); // colSum = A * vector
    maxColSum = colSum[cblas_isamax(N, colSum, 1)];

    std::fill(vector, vector + N, 1);
    cblas_sgemv(CblasRowMajor, CblasTrans, N, N,
                1, A, N, vector, 1, 0, colSum, 1); // rowSum = A(-1) * vector
    maxRowSum = colSum[cblas_isamax(N, colSum, 1)];

    cblas_scopy(N * N, A, 1, B, 1);
    cblas_sscal(N * N, 1 / (maxColSum * maxRowSum), B, 1); // Calculate B matrix

    for (i = 0; i < N; i++) { // Create indent matrices
        matrixSum[i * N + i] = 1;
        Rtmp[i * N + i] = 1;
    }

    clock_t matMulTime = -clock();
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N,
                -1, B, N, A, N, 1, Rtmp, N); // Rtmp= -BA + Rtmp
    matMulTime += clock();
    std::cout << "matMulTime: " << matMulTime / (float) CLOCKS_PER_SEC << std::endl;
    cblas_scopy(N * N, Rtmp, 1, R, 1); // R = Rtmp

    for (i = 0; i < 11; i++) { // I + R + R^2 +...
        cblas_saxpy(N * N, 1, Rtmp, 1, matrixSum, 1); // matrixSum += Rtmp
        cblas_scopy(N * N, Rtmp, 1, Rmul, 1); // Rmul = Rtmp
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N,
                    1, Rmul, N, R, N, 0, Rtmp, N); // Rtmp = R^(i+1)
    }
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N,
                1, matrixSum, N, B, N, 0, reversedMatrix, N); // A^(-1) = (I + R + R^2 + ...)B
}

int main() {
    float inputMatrix[N * N];
    float outputMatrix[N * N];
    srand(time(nullptr));
    for (int i = 0; i < N; i++) { // Initialize some matrix
        for (int j = 0; j < N; j++) {
            inputMatrix[i * N + j] = int(rand()) % 10;
        }
    }
    /*for (int i = 0; i < N; i++) { // Initialize test matrix
        for (int j = 0; j < N; j++) {
            if (i == j) {
                inputMatrix[i * N + j] = 10;
            } else {
                inputMatrix[i * N + j] = 1;
            }
        }
    }*/

    PrintMat(inputMatrix);
    clock_t elapsedTime = -clock();
    InverseMatrix(inputMatrix, outputMatrix);
    elapsedTime += clock();
    PrintMat(outputMatrix);

    std::cout << "All time: " << elapsedTime / (float) CLOCKS_PER_SEC << std::endl;
    return 0;
}