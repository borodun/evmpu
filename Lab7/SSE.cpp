#include <ctime>
#include <xmmintrin.h>
#include <iostream>
#include <algorithm>

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

void TransposeMatrix(float A[N][N]) {
    for (int i = 0; i < N; i += 4) {
        for (int j = 0; j < N; j += 4) {
            if (i >= j) {
                __m128 row11 = _mm_loadu_ps(&A[i + 0][j]);
                __m128 row12 = _mm_loadu_ps(&A[i + 1][j]);
                __m128 row13 = _mm_loadu_ps(&A[i + 2][j]);
                __m128 row14 = _mm_loadu_ps(&A[i + 3][j]);
                _MM_TRANSPOSE4_PS(row11, row12, row13, row14);
                __m128 row21 = _mm_loadu_ps(&A[j + 0][i]);
                __m128 row22 = _mm_loadu_ps(&A[j + 1][i]);
                __m128 row23 = _mm_loadu_ps(&A[j + 2][i]);
                __m128 row24 = _mm_loadu_ps(&A[j + 3][i]);
                _MM_TRANSPOSE4_PS(row21, row22, row23, row24);
                _mm_storeu_ps(&A[i + 0][j], row21);
                _mm_storeu_ps(&A[i + 1][j], row22);
                _mm_storeu_ps(&A[i + 2][j], row23);
                _mm_storeu_ps(&A[i + 3][j], row24);
                _mm_storeu_ps(&A[j + 0][i], row11);
                _mm_storeu_ps(&A[j + 1][i], row12);
                _mm_storeu_ps(&A[j + 2][i], row13);
                _mm_storeu_ps(&A[j + 3][i], row14);
            }
        }
    }
}

void MultiplyMatrices(float A[N][N], float B[N][N], float C[N][N]) {
    TransposeMatrix(B);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; j += 4) {
            __m128 vR = _mm_setzero_ps();
            for (int k = 0; k < N; k++) {
                //C[i][j] += A[i][k] * B[k][j];
                __m128 vA = _mm_set1_ps(A[i][k]);
                __m128 vB = _mm_loadu_ps(&B[k][j]);
                vR = _mm_add_ps(vR, _mm_mul_ps(vA, vB));
            }
            _mm_storeu_ps(&C[i][j], vR);
        }
    }
    TransposeMatrix(B);
}

void SumMatrices(float A[N][N], float B[N][N], float C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k += 4) {
            __m128 vA = _mm_loadu_ps(&A[i][k]);
            __m128 vB = _mm_loadu_ps(&B[i][k]);
            _mm_storeu_ps(&C[i][k], _mm_add_ps(vA, vB));
        }
    }
}

void SubMatrices(float A[N][N], float B[N][N], float C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k += 4) {
            __m128 vA = _mm_loadu_ps(&A[i][k]);
            __m128 vB = _mm_loadu_ps(&B[i][k]);
            _mm_storeu_ps(&C[i][k], _mm_sub_ps(vA, vB));
        }
    }
}

float VectorFindMax(__m128 vA) {
    float result[4];
    _mm_storeu_ps(result, vA);
    return *std::max_element(result, result + 4);
}

float FindMaxColSum(float A[N][N]) {
    float max = 0;
    for (int i = 0; i < N; i += 4) {
        __m128 row1;
        __m128 vA = _mm_setzero_ps();
        for (int j = 0; j < N; j++) {
            row1 = _mm_loadu_ps(&A[j][i]);
            vA = _mm_add_ps(vA, row1);
        }
        if (VectorFindMax(vA) > max) {
            max = VectorFindMax(vA);
        }
    }
    return max;
}

void InvertMatrix(float A[N][N], float reversedMatrix[N][N]) {
    float I[N][N] = {0};
    float B[N][N] = {0};
    float R[N][N] = {0};
    float Rtmp[N][N] = {0};
    float tmpMatrix[N][N] = {0};
    float matrixSum[N][N] = {0};
    int i, j;

    float maxColSum = FindMaxColSum(A);
    TransposeMatrix(A);
    float maxRowSum = FindMaxColSum(A);
    for (i = 0; i < N; i++) { // Calculate B matrix
        for (j = 0; j < N; j += 4) {
            __m128 vA = _mm_loadu_ps(&A[i][j]);
            __m128 vB = _mm_set_ps1(maxColSum * maxRowSum);
            _mm_storeu_ps(&B[i][j], _mm_div_ps(vA, vB));
        }
    }
    TransposeMatrix(A);
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
        SumMatrices(tmpMatrix, matrixSum, matrixSum); // matrixSum += R^(n+2)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j += 4) {
                __m128 vA = _mm_loadu_ps(&tmpMatrix[i][j]);
                _mm_storeu_ps(&Rtmp[i][j], vA);
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
    InvertMatrix(inputMatrix, outputMatrix);
    elapsedTime += clock();
    PrintMat(outputMatrix);

    std::cout << "All time: " << elapsedTime / (float) CLOCKS_PER_SEC << std::endl;
    return 0;
}