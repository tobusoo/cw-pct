#include <assert.h>
#include <math.h>
#include <mpi.h>

#include <jacobi_mpi.h>

void get_chunk(int a, int b, int commsize, int rank, int* lb, int* ub)
{
    int n = b - a + 1;
    int q = n / commsize;
    if (n % commsize)
        q++;
    int r = commsize * q - n;

    int chunk = q;
    if (rank >= commsize - r)
        chunk = q - 1;

    *lb = a;
    if (rank > 0) {
        if (rank <= commsize - r)
            *lb += q * rank;
        else
            *lb += q * (commsize - r) + (q - 1) * (rank - (commsize - r));
    }
    *ub = *lb + chunk - 1;
}

bool can_use_jacobi_mpi(const double* a, const int n, const int lb, const int nrows)
{
    for (int i = 0; i < nrows; i++) {
        if (a[i * n + i + lb] == 0)
            return false;

        double row_sum = -fabs(a[i * n + i + lb]);
        for (int j = 0; j < nrows; j++) {
            row_sum += fabs(a[i * n + j]);
        }

        if (row_sum >= fabs(a[i * n + i + lb]))
            return false;
    }

    return true;
}

void jacobi_mpi(const double* a, const double* b, double* x, const int n, const double eps)
{
    double delta = eps + 1, delta_local;
    int lb, ub, commsize, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    get_chunk(0, n - 1, commsize, rank, &lb, &ub);
    int nrows = ub - lb + 1;

    double* temp = malloc(sizeof(*temp) * nrows);
    int* displs = malloc(sizeof(*displs) * commsize);
    int* recvcounts = malloc(sizeof(*recvcounts) * commsize);
    assert(temp && displs && recvcounts && "not enough memory");

    int offset = 0;
    for (int i = 0; i < commsize; i++) {
        int lb, ub;
        get_chunk(0, n - 1, commsize, i, &lb, &ub);

        recvcounts[i] = ub - lb + 1;
        displs[i] = offset;
        offset += recvcounts[i];
    }

    MPI_Request reqs[2];
    while (delta > eps) {
        for (int i = 0; i < nrows; i++) {
            temp[i] = b[i];
            for (int j = 0; j < n; j++) {
                temp[i] -= (i + lb != j) ? a[i * n + j] * x[j] : 0;
            }
            temp[i] /= a[i * n + i + lb];
        }

        delta_local = fabs(temp[0] - x[lb]);
        for (int j = 1; j < nrows; j++) {
            delta_local = fmax(delta_local, fabs(temp[j] - x[j + lb]));
        }
        MPI_Iallreduce(&delta_local, &delta, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD, &reqs[0]);
        MPI_Iallgatherv(
                temp,
                nrows,
                MPI_DOUBLE,
                x,
                recvcounts,
                displs,
                MPI_DOUBLE,
                MPI_COMM_WORLD,
                &reqs[1]);
        MPI_Waitall(2, reqs, MPI_STATUS_IGNORE);
    }

    free(recvcounts);
    free(displs);
    free(temp);
}
