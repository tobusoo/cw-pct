#include <stdio.h>
#include <stdlib.h>

#include <jacobi.h>

int main()
{
    double** a = malloc(sizeof(double*) * 3);
    for (size_t i = 0; i < 3; i++) {
        a[i] = malloc(sizeof(double) * 3);
    }
    a[0][0] = 3;
    a[0][1] = 2;
    a[0][2] = 1;

    a[1][0] = 1;
    a[1][1] = 2;
    a[1][2] = 1;

    a[2][0] = 4;
    a[2][1] = 3;
    a[2][2] = -2;

    double b[3] = {10, 8, 4};
    double x[3] = {1, 1, 1};
    jacobi(a, b, x, 3, 1e-6);

    for (size_t i = 0; i < 3; i++) {
        printf("%lf ", x[i]);
    }
    printf("\n");

    return 0;
}