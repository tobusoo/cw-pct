#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifdef COMPARE_WITH_GSL
#include <gsl/gsl_linalg.h>
#endif

#include <jacobi.h>

#define PRINT_INFO 0

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void initialize(double* a, double* b, int n)
{
    for (int i = 0; i < n; i++) {
        srand(i * (n + 1));

        double row_sum = 0;
        for (int j = 0; j < n; j++) {
            a[i * n + j] = rand() % 100 + 1;
            row_sum += a[i * n + j];
        }

        a[i * n + i] += row_sum + (rand() % 50 + 10);
        b[i] = rand() % 100 + 1;
    }
}

#ifdef COMPARE_WITH_GSL
void compare(double* a, double* b, double* x, int n, double eps)
{
    int s;
    gsl_matrix_view gsl_a = gsl_matrix_view_array(a, n, n);
    gsl_vector_view gsl_b = gsl_vector_view_array(b, n);
    gsl_vector* gsl_x = gsl_vector_alloc(n);

    gsl_permutation* p = gsl_permutation_alloc(n);
    gsl_linalg_LU_decomp(&gsl_a.matrix, p, &s);
    gsl_linalg_LU_solve(&gsl_a.matrix, p, &gsl_b.vector, gsl_x);

#if PRINT_INFO
    printf("GSL    X[%d]: ", n);
    for (int i = 0; i < n; i++)
        printf("%f ", gsl_vector_get(gsl_x, i));
    printf("\n");
#endif

    for (int i = 0; i < n; i++) {
        if (fabs(x[i] - gsl_vector_get(gsl_x, i)) > eps) {
            fprintf(stderr, "Invalid result: elem %d: %f %f\n", i, x[i], gsl_vector_get(gsl_x, i));
            break;
        }
    }

    gsl_permutation_free(p);
    gsl_vector_free(gsl_x);
}
#endif

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: jacobi <n>\n");
        return EXIT_FAILURE;
    }
    double total_time = -wtime();

    const double eps = 1e-6;
    const int n = atoll(argv[1]);
    double* a = malloc(sizeof(*a) * n * n);
    double* b = malloc(sizeof(*b) * n);
    double* x = calloc(sizeof(*x), n);
    assert(a && b && x && "not enough memory");

    initialize(a, b, n);
    if (!can_use_jacobi(a, n)) {
        fprintf(stderr, "matrix A can't be used for Jacobi method\n");
        return EXIT_FAILURE;
    }
    jacobi(a, b, x, n, eps);

#if PRINT_INFO
    printf("JACOBI X[%d]: ", n);
    for (int i = 0; i < n; i++) {
        printf("%lf ", x[i]);
    }
    printf("\n");
#endif

#if COMPARE_WITH_GSL
    initialize(a, b, n);
    compare(a, b, x, n, eps);
#endif
    total_time += wtime();
    printf("Jacobi (serial):\n[n=%d] time (sec): %.6lf\n", n, total_time);

    free(a);
    free(b);
    free(x);
    return 0;
}
