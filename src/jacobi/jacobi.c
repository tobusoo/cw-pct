#include <assert.h>

#include <math.h>
#include <stdio.h>

#include <jacobi.h>

bool can_use_jacobi(const double* a, const int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i * n + i] == 0)
            return false;

        double row_sum = -fabs(a[i * n + i]);
        for (int j = 0; j < n; j++) {
            row_sum += fabs(a[i * n + j]);
        }

        if (row_sum >= fabs(a[i * n + i]))
            return false;
    }

    return true;
}

void jacobi(const double* a, const double* b, double* x, const int n, const double eps)
{
    double delta = eps + 1;
    double* temp = malloc(sizeof(*temp) * n);
    assert(temp && "not enough memory");

    while (delta > eps) {
        for (int i = 0; i < n; i++) {
            temp[i] = b[i];
            for (int j = 0; j < n; j++) {
                temp[i] -= (i != j) ? a[i * n + j] * x[j] : 0;
            }
            temp[i] /= a[i * n + i];
        }

        delta = fabs(temp[0] - x[0]);
        x[0] = temp[0];
        for (int j = 1; j < n; j++) {
            delta = fmax(delta, fabs(temp[j] - x[j]));
            x[j] = temp[j];
        }
    }

    free(temp);
}
