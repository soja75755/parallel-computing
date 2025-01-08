#include <immintrin.h>
#include "matmul.h"

#define MAX_TRANSPOSE_BUFFER 10 * 1024 * 1024
float transpose_tmp[MAX_TRANSPOSE_BUFFER];

inline void simd_mul_fp_256(const float *a, const float *b, float *c)
{
    __m256 acc = _mm256_fmadd_ps(_mm256_load_ps(a), _mm256_load_ps(b),
                                 _mm256_load_ps(c));
    _mm256_store_ps(c, acc);
}

void simd_mat_mul(const struct matmul_params *params)
{
    int i, j, k;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    float *data_A = A->data_ptr, *data_B = B->data_ptr, *data_C = C->data_ptr;
    check_matrices(A, B, C);

    // transpose the B
    for (i = 0; i < B->column; i++)
        for (j = 0; j < B->row; j++)
            transpose_tmp[i * B->row + j] = data_B[j * B->column + i];

    for (i = 0; i < C->row; i++)
        for (j = 0; j < C->column; j++) {
            float accumulators[8] = {};
            for (k = 0; k < A->column; k += 8)
                simd_mul_fp_256(&data_A[i * A->column + k],
                                &transpose_tmp[j * B->row + k], accumulators);
            data_C[i * C->column + j] = accumulators[0] + accumulators[1] +
                                        accumulators[2] + accumulators[3] +
                                        accumulators[4] + accumulators[5] +
                                        accumulators[6] + accumulators[7];
        }
}