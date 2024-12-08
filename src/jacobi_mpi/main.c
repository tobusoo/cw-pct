#include <assert.h>
#include <mpi.h>
#include <stdio.h>

#include <jacobi_mpi.h>

#define PRINT_INFO 1

void initialize(double* a, double* b, int n, int commsize, int rank)
{
    int lb, ub;
    get_chunk(0, n - 1, commsize, rank, &lb, &ub);
    int nrows = ub - lb + 1;

    for (int i = 0; i < nrows; i++) {
        srand((lb + i) * (n + 1));

        double row_sum = 0;
        for (int j = 0; j < n; j++) {
            a[i * n + j] = rand() % 100 + 1;
            row_sum += a[i * n + j];
        }

        a[i * n + i + lb] += row_sum + (rand() % 50 + 10);
        b[i] = rand() % 100 + 1;
    }
}

int main(int argc, char* argv[])
{
    const double eps = 1e-6;
    int commsize, rank;
    double total_time = -MPI_Wtime();

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    if (rank == 0) {
        n = (argc > 1) ? atoll(argv[1]) : 100;
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int lb, ub;
    get_chunk(0, n - 1, commsize, rank, &lb, &ub);
    int nrows = ub - lb + 1;

    double* a = malloc(sizeof(*a) * n * nrows);
    double* b = malloc(sizeof(*b) * nrows);
    double* x = calloc(sizeof(*x), n);
    assert(a && b && x && "not enough memory");

    initialize(a, b, n, commsize, rank);
    if (!can_use_jacobi_mpi(a, n, lb, nrows)) {
        fprintf(stderr, "matrix A can't be used for Jacobi method\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    jacobi_mpi(a, b, x, n, eps);

    double total_max;
    total_time += MPI_Wtime();
    MPI_Reduce(&total_time, &total_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

#if PRINT_INFO
    if (rank == 0) {
        printf("JACOBI X[%d]: ", n);
        for (int i = 0; i < n; i++) {
            printf("%lf ", x[i]);
        }
        printf("\n");
    }
#endif

    if (rank == 0) {
        printf("Jacobi (%d procs):\n", commsize);
        printf("[n=%d] time (sec) %.6lf\n", n, total_max);
    }

    free(a);
    free(b);
    free(x);
    MPI_Finalize();
    return 0;
}
