#include "matmul.h"

void naive_mat_mul(const struct matmul_params *params)
{
    int i, j, k;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    float *data_A = A->data_ptr, *data_B = B->data_ptr, *data_C = C->data_ptr;
    check_matrices(A, B, C);

    for (i = 0; i < C->row; i++) {
        for (j = 0; j < C->column; j++) {
            {
                float acc = 0;
                for (k = 0; k < A->column; k++) {
                    acc +=
                        data_A[i * A->column + k] * data_B[k * B->column + j];
                }
                data_C[i * C->column + j] = acc;
            }
        }
    }
}
