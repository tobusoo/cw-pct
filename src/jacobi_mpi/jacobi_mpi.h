#pragma once
#include <stdbool.h>
#include <stdlib.h>

void jacobi_mpi(const double* a, const double* b, double* x, const size_t n, const double eps);
bool can_use_jacobi_mpi(const double* a, const size_t n);