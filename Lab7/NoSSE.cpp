#include <ctime>
#include <iostream>

#define N 2048
#define PRINT_N 4

void PrintMat(float A[N][N]) {
    for (int i = 0; i < PRINT_N; ++i) {
        for (int j = 0; j < PRINT_N; ++j) {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void MultiplyMatrices(const float A[N][N], const float B[N][N], float C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void SumMatrices(const float A[N][N], const float B[N][N], float C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void SubMatrices(const float A[N][N], const float B[N][N], float C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

void InverseMatrix(float A[N][N], float reversedMatrix[N][N]) {
    float maxColSum = 0, maxRowSum = 0;
    float I[N][N] = {0};
    float B[N][N] = {0};
    float R[N][N] = {0};
    float Rtmp[N][N] = {0};
    float tmpMatrix[N][N] = {0};
    float matrixSum[N][N] = {0};
    int i, j;

    for (i = 0; i < N; i++) { // Max sum per column and per row
        float tmpSum1 = 0;
        float tmpSum2 = 0;
        for (j = 0; j < N; j++) {
            tmpSum1 = tmpSum1 + A[i][j];
            tmpSum2 = tmpSum2 + A[j][i];
        }
        if (tmpSum1 > maxColSum) {
            maxColSum = tmpSum1;
        }
        if (tmpSum2 > maxRowSum) {
            maxRowSum = tmpSum2;
        }
    }

    for (i = 0; i < N; i++) { // Calculate B matrix
        for (j = 0; j < N; j++) {
            B[i][j] = A[j][i] / (maxColSum * maxRowSum);
        }
    }
    for (i = 0; i < N; i++) { // Create indent matrix
        I[i][i] = 1;
    }

    clock_t matMulTime = -clock();
    MultiplyMatrices(B, A, tmpMatrix);// tmpMatrix = BA
    matMulTime += clock();
    std::cout << "matMulTime: " << matMulTime / (float) CLOCKS_PER_SEC << std::endl;

    SubMatrices(I, tmpMatrix, R); // R = I - BA
    SubMatrices(I, tmpMatrix, Rtmp); // Rtmp = R

    SumMatrices(I, R, matrixSum);
    for (int n = 0; n < 10; n++) { // I + R + R^2 +...
        MultiplyMatrices(R, Rtmp, tmpMatrix); // R^(n+2)
        SumMatrices(tmpMatrix, matrixSum, matrixSum); // matrixSum+= R^(n+2)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                Rtmp[i][j] = tmpMatrix[i][j];
            }
        }
    }
    MultiplyMatrices(matrixSum, B, reversedMatrix); // A^(-1) = (I + R + R^2 + ...)B
}

int main() {
    float inputMatrix[N][N];
    float outputMatrix[N][N];
    srand(time(nullptr));
    for (auto &row : inputMatrix) { // Initialize some matrix
        for (float &element : row) {
            element = int(rand()) % 10;
        }
    }
    /*for (int i = 0; i < N; i++) { // Initialize test matrix
        for (int j = 0; j < N; j++) {
            if (i == j) {
                inputMatrix[i][j] = 10;
            } else {
                inputMatrix[i][j] = 1;
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