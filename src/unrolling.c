#include "matmul.h"
void unrolling_mat_mul(const struct matmul_params *params)
{
    int i, j, k;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    float *data_A = A->data_ptr, *data_B = B->data_ptr, *data_C = C->data_ptr;
    check_matrices(A, B, C);

    for (i = 0; i < C->row; i++) {
        // cache line size 64 byte
        for (j = 0; j < C->column; j += 16) {
            {
                float acc0 = 0;
                float acc1 = 0;
                float acc2 = 0;
                float acc3 = 0;
                float acc4 = 0;
                float acc5 = 0;
                float acc6 = 0;
                float acc7 = 0;
                float acc8 = 0;
                float acc9 = 0;
                float acc10 = 0;
                float acc11 = 0;
                float acc12 = 0;
                float acc13 = 0;
                float acc14 = 0;
                float acc15 = 0;
                for (k = 0; k < A->column; k += 1) {
                    float Aik0 = data_A[i * A->column + k + 0];
                    acc0 += Aik0 * data_B[(k + 0) * B->column + (j + 0)];
                    acc1 += Aik0 * data_B[(k + 0) * B->column + (j + 1)];
                    acc2 += Aik0 * data_B[(k + 0) * B->column + (j + 2)];
                    acc3 += Aik0 * data_B[(k + 0) * B->column + (j + 3)];
                    acc4 += Aik0 * data_B[(k + 0) * B->column + (j + 4)];
                    acc5 += Aik0 * data_B[(k + 0) * B->column + (j + 5)];
                    acc6 += Aik0 * data_B[(k + 0) * B->column + (j + 6)];
                    acc7 += Aik0 * data_B[(k + 0) * B->column + (j + 7)];
                    acc8 += Aik0 * data_B[(k + 0) * B->column + (j + 8)];
                    acc9 += Aik0 * data_B[(k + 0) * B->column + (j + 9)];
                    acc10 += Aik0 * data_B[(k + 0) * B->column + (j + 10)];
                    acc11 += Aik0 * data_B[(k + 0) * B->column + (j + 11)];
                    acc12 += Aik0 * data_B[(k + 0) * B->column + (j + 12)];
                    acc13 += Aik0 * data_B[(k + 0) * B->column + (j + 13)];
                    acc14 += Aik0 * data_B[(k + 0) * B->column + (j + 14)];
                    acc15 += Aik0 * data_B[(k + 0) * B->column + (j + 15)];
                }
                data_C[i * C->column + j + 0] = acc0;
                data_C[i * C->column + j + 1] = acc1;
                data_C[i * C->column + j + 2] = acc2;
                data_C[i * C->column + j + 3] = acc3;
                data_C[i * C->column + j + 4] = acc4;
                data_C[i * C->column + j + 5] = acc5;
                data_C[i * C->column + j + 6] = acc6;
                data_C[i * C->column + j + 7] = acc7;
                data_C[i * C->column + j + 8] = acc8;
                data_C[i * C->column + j + 9] = acc9;
                data_C[i * C->column + j + 10] = acc10;
                data_C[i * C->column + j + 11] = acc11;
                data_C[i * C->column + j + 12] = acc12;
                data_C[i * C->column + j + 13] = acc13;
                data_C[i * C->column + j + 14] = acc14;
                data_C[i * C->column + j + 15] = acc15;
            }
        }
    }
}
