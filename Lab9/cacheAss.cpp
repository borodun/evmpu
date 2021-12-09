#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <intrin.h>

#define TIME_MEASURE_COUNT 100
#define MEASURE_AMOUNT 100

double MeasureTime(const int *array, int size) {
    unsigned long long minDuration = ULONG_LONG_MAX;
    int k;
    for (int i = 0; i < TIME_MEASURE_COUNT; ++i) {
        unsigned long long start = __rdtsc();
        k = 0;
        for (int j = 0; j < size; ++j) {
            k = array[k];
        }
        unsigned long long end = __rdtsc();
        unsigned long long tmpDuration = end - start;
        if (tmpDuration < minDuration) {
            minDuration = tmpDuration;
        }
    }
    if (k) {
        std::cerr << " ";
    }
    return minDuration / double(size);
}

int main() {
    int i, j;
    int cacheSize = 256 * 16;
    int offset = 256 * 1024 * 2;

    std::ofstream output("../file.csv");
    if (!output) {
        std::cerr << "Can not open output file";
        return 0;
    }
    output << ";ticks" << std::endl;

    for (int n = 1; n <= 32; ++n) {
            int array[offset * n];
            std::cout << "memory: " << offset * n / 256 << "KB fragmentNum:" << n << std::endl;
            for (i = 0; i < n - 1; i++) {
                for (j = 0; j < cacheSize / n; j++) {
                    array[i * offset + j] = (i + 1) * offset + j;
                }
            }
            for (j = 0; j < cacheSize / n - 1; j++) {
                array[(n - 1) * offset + j] = j + 1;
            }
            array[(n - 1) * offset + j] = 0;

            double ticks = 0;
            for (i = 0; i < MEASURE_AMOUNT; ++i) {
                ticks += MeasureTime(array, cacheSize);
            }

            std::cout << ticks / MEASURE_AMOUNT << std::endl;
            output << n  << ";" << ticks / MEASURE_AMOUNT << std::endl;
    }

}
