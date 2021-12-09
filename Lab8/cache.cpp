#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <intrin.h>

#define TIME_MEASURE_COUNT 10
#define REPEAT 2
#define MEASURE_AMOUNT 10

using namespace std;

double MeasureTime(const int *array, int size) {
    unsigned long long minDuration = ULONG_LONG_MAX;
    int k;
    for (int i = 0; i < TIME_MEASURE_COUNT; ++i) {
        unsigned long long start = __rdtsc();
        k = 0;
        for (int j = 0; j < size * REPEAT; ++j) {
            k = array[k];
        }
        unsigned long long end = __rdtsc();
        unsigned long long tmpDuration = end - start;
        if (tmpDuration < minDuration) {
            minDuration = tmpDuration;
        }
    }
    cerr << k << endl;
    return minDuration / double(size * REPEAT);
}

int main() {
    int n = 256;
    int mMax = 1024 * 16;

    ofstream output("file8.csv");
    if (!output) {
        cout << "Can not open output file";
        return 0;
    }
    output << ";direct;reverse;random" << endl;

    for (int m = 1, iter = 1; m < mMax; m += 16, ++iter) {
        int size = n * m;
        long elapsedTime = -clock();
        int directTraversal[size];
        int reverseTraversal[size];
        int randomTraversal[size];
        int shuffledArray[size];

        for (int i = 0; i < size; ++i) {
            shuffledArray[i] = i;
            directTraversal[i] = i + 1;
            reverseTraversal[size - i - 1] = size - i - 2;
        }
        directTraversal[size - 1] = 0;
        reverseTraversal[0] = size - 1;
        shuffle(shuffledArray, shuffledArray + size, default_random_engine(time(nullptr)));

        for (int i = 0; i < size - 1; ++i) {
            randomTraversal[shuffledArray[i]] = shuffledArray[i + 1];
        }
        randomTraversal[shuffledArray[size - 1]] = shuffledArray[0];

        double directDuration = 0;
        double reverseDuration = 0;
        double randomDuration = 0;

        for (int i = 0; i < MEASURE_AMOUNT; ++i) {
            directDuration += MeasureTime(directTraversal, size);
            reverseDuration += MeasureTime(reverseTraversal, size);
            randomDuration += MeasureTime(shuffledArray, size);
        }

        cout << setprecision(numeric_limits<double>::digits10 + 1);
        cout << endl << "iter: " << iter << " memoryPerArray: " << size / 256 << "KB" << endl
             << "n: " << n << " m: " << m << endl
             << "directDuration: " << directDuration / MEASURE_AMOUNT << endl
             << "reverseDuration: " << reverseDuration / MEASURE_AMOUNT << endl
             << "randomDuration: " << randomDuration / MEASURE_AMOUNT << endl;

        output << size / 256 << ";"
               << directDuration / MEASURE_AMOUNT << ";"
               << reverseDuration / MEASURE_AMOUNT << ";"
               << randomDuration / MEASURE_AMOUNT << endl;
        elapsedTime += clock();
        cout << "elapsedTime: " << elapsedTime / double(CLOCKS_PER_SEC) << endl;
    }
    return 0;
}
