#include <stdio.h>
#include <math.h>

// Функция для обычной корреляции
double correlation(int x[], int y[], int N) {
    double result = 0;
    for (int i = 0; i < N; i++) {
        result += x[i] * y[i];
    }
    return result;
}

// Функция для нормализованной корреляции
double normalized_correlation(int x[], int y[], int N) {
    double numerator = 0, sum_x_sq = 0, sum_y_sq = 0;
    
    for (int i = 0; i < N; i++) {
        numerator += x[i] * y[i];
        sum_x_sq += x[i] * x[i];
        sum_y_sq += y[i] * y[i];
    }
    
    return numerator / (sqrt(sum_x_sq) * sqrt(sum_y_sq));
}

int main() {
    int a[] = {3, 4, 7, 8, 3, -2, -4, 0};
    int b[] = {2, 5, 8, 10, 4, -3, -1, 2};
    int c[] = {-2, 0, -3, -7, 2, -3, 5, 9};
    int N = 8;

    printf("correlation a and b: %.2f\n", correlation(a, b, N));
    printf("correlation a and c: %.2f\n", correlation(a, c, N));
    printf("correlation b and c: %.2f\n", correlation(b, c, N));

    printf("normalized correlation a and b: %.2f\n", normalized_correlation(a, b, N));
    printf("normalized correlation a and c: %.2f\n", normalized_correlation(a, c, N));
    printf("normalized correlation b and c: %.2f\n", normalized_correlation(b, c, N));

    return 0;
}
