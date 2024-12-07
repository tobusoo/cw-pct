#include <assert.h>
#include <math.h>

#include <jacobi.h>

void jacobi(double** a, double* b, double* x, size_t n, double eps)
{
    double delta = eps + 1;
    double* temp = malloc(sizeof(double) * n);
    assert(temp && "not enough memory");

    while (delta > eps) {
        for (size_t i = 0; i < n; i++) {
            temp[i] = b[i];
            for (size_t j = 0; j < n; j++) {
                temp[i] -= (i != j) ? a[i][j] * x[j] : 0;
            }
            temp[i] /= a[i][i];
        }

        delta = fabs(temp[0] - x[0]);
        for (size_t j = 1; j < n; j++) {
            delta = fmax(delta, fabs(temp[j] - x[j]));
            x[j] = temp[j];
        }
    }

    free(temp);
}
