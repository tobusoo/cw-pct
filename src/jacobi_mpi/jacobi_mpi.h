#pragma once
#include <stdbool.h>
#include <stdlib.h>

void get_chunk(int a, int b, int commsize, int rank, int* lb, int* ub);
void jacobi_mpi(const double* a, const double* b, double* x, const int n, const double eps);
bool can_use_jacobi_mpi(const double* a, const int n, const int lb, const int nrows);