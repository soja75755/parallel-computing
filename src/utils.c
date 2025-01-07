#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "matmul.h"

void initialize_matrix(float *M, int size)
{
    for (int i = 0; i < size; i++) {
        M[i] = (float) (rand()) / (float) (RAND_MAX);
    }
}

void check_matrices(const struct matrix *A,
                    const struct matrix *B,
                    const struct matrix *C)
{
    assert(A->column == B->row);
    assert(B->column == C->column);
    assert(C->row == A->row);
}

float interval_to_ms(struct timeval *start, struct timeval *end)
{
    float usec = (end->tv_sec - start->tv_sec) * 1000000 +
                 (end->tv_usec - start->tv_usec);
    return usec / 1000;
}

void evaluate(enum IMP_TYPE type, const struct matmul_params *params)
{
    struct timeval start, end;
    int ms;
    const char *function_name;

    gettimeofday(&start, NULL);
    switch (type) {
    case NAIVE:
        function_name = "naive_mat_mul";
        naive_mat_mul(params);
        break;
    case UNROLLING:
        function_name = "unrolling_mat_mul";
        unrolling_mat_mul(params);
        break;
    case REORDERING:
        function_name = "reordering_mat_mul";
        reordering_mat_mul(params);
        break;
    case TILING:
        function_name = "tiling_mat_mul";
        tiling_mat_mul(params);
        break;
    case MULTITHREADING:
        function_name = "multithreading_mat_mul";
        multithreading_mat_mul(params);
        break;
    }
    gettimeofday(&end, NULL);
    ms = interval_to_ms(&start, &end);
    printf("%s : %d\n", function_name, ms);
}
