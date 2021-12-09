#include <iostream>
#include <ctime>
#include <cmath>

long double LeibnizFormula(int n) {
    long double pi = 0;
    for (int i = 0; i < n; ++i) {
        long double appendix  = 1 / (long double) (2 * i + 1);
        pi += appendix*pow(-1,i) ;
    }
    pi *= 4;
    return pi;
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    long double startTime = clock();
    long double pi = LeibnizFormula(n);
    long double endTime = clock();
    long double runTime = endTime - startTime;
    std::cout << "N = " << n << std::endl;
    std::cout << "Pi = " << pi << std::endl;
    std::cout << "Time = " << runTime/1000000 << "sec"<< std::endl;
    return 0;
}
