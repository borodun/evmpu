#include <cmath>
#include <cstdlib>
#include <ctime>

long double LeibnizFormula(int n) {
    long double pi = 0;
    for (int i = 0; i < n; ++i) {
        long double appendix  = 1 / (long double) (2 * i + 1);
        pi += appendix * pow(-1, i);
    }
    pi *= 4;
    return pi;
}

int main() {
    int N = 1000000;
    long double startTime = clock();
    long double pi = LeibnizFormula(N);
    long double endTime = clock();
    long double runTIme = endTime - startTime;
    return 0;
}