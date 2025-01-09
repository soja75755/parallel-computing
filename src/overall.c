#include <assert.h>
#include <immintrin.h>
#include <pthread.h>
#include "matmul.h"

#define MAX_TRANSPOSE_BUFFER 10 * 1024 * 1024
float overall_transpose_tmp[MAX_TRANSPOSE_BUFFER];

void *overall_thread_func(void *args)
{
    const int BLK_SIZE = 16;
    const int SIMD_SIZE = 8;

    struct thread_args *mat_args = (struct thread_args *) args;
    const struct matrix *A = mat_args->A, *B = mat_args->B, *C = mat_args->C;
    float *data_A = A->data_ptr, *data_B = B->data_ptr, *data_C = C->data_ptr;
    int start_i = mat_args->start_i, end_i = mat_args->end_i;


    assert((end_i - start_i) % BLK_SIZE == 0);
    assert(A->column % BLK_SIZE == 0);
    assert(C->column % BLK_SIZE == 0);

    for (int ti = start_i; ti < end_i; ti += BLK_SIZE)
        for (int tj = 0; tj < C->column; tj += BLK_SIZE) {
            for (int i = ti; i < ti + BLK_SIZE; i++)
                for (int j = tj; j < tj + BLK_SIZE; j += 8) {
                    float acc0[8] = {};
                    float acc1[8] = {};
                    float acc2[8] = {};
                    float acc3[8] = {};
                    float acc4[8] = {};
                    float acc5[8] = {};
                    float acc6[8] = {};
                    float acc7[8] = {};
                    __m256 *acc0_fp_256 = (__m256 *) acc0;
                    __m256 *acc1_fp_256 = (__m256 *) acc1;
                    __m256 *acc2_fp_256 = (__m256 *) acc2;
                    __m256 *acc3_fp_256 = (__m256 *) acc3;
                    __m256 *acc4_fp_256 = (__m256 *) acc4;
                    __m256 *acc5_fp_256 = (__m256 *) acc5;
                    __m256 *acc6_fp_256 = (__m256 *) acc6;
                    __m256 *acc7_fp_256 = (__m256 *) acc7;
                    for (int k = 0; k < A->column; k += 8) {
                        __m256 Aik0 =
                            _mm256_load_ps(&data_A[i * A->column + k + 0]);
                        *acc0_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 0) * B->column + k]),
                            *acc0_fp_256);
                        *acc1_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 1) * B->column + k]),
                            *acc1_fp_256);
                        *acc2_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 2) * B->column + k]),
                            *acc2_fp_256);
                        *acc3_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 3) * B->column + k]),
                            *acc3_fp_256);
                        *acc4_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 4) * B->column + k]),
                            *acc4_fp_256);
                        *acc5_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 5) * B->column + k]),
                            *acc5_fp_256);
                        *acc6_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 6) * B->column + k]),
                            *acc6_fp_256);
                        *acc7_fp_256 = _mm256_fmadd_ps(
                            Aik0,
                            _mm256_load_ps(&data_B[(j + 7) * B->column + k]),
                            *acc7_fp_256);
                    }
                    data_C[i * C->column + j + 0] =
                        acc0[0] + acc0[1] + acc0[2] + acc0[3] + acc0[4] +
                        acc0[5] + acc0[6] + acc0[7];
                    data_C[i * C->column + j + 1] =
                        acc1[0] + acc1[1] + acc1[2] + acc1[3] + acc1[4] +
                        acc1[5] + acc1[6] + acc1[7];
                    data_C[i * C->column + j + 2] =
                        acc2[0] + acc2[1] + acc2[2] + acc2[3] + acc2[4] +
                        acc2[5] + acc2[6] + acc2[7];
                    data_C[i * C->column + j + 3] =
                        acc3[0] + acc3[1] + acc3[2] + acc3[3] + acc3[4] +
                        acc3[5] + acc3[6] + acc3[7];
                    data_C[i * C->column + j + 4] =
                        acc4[0] + acc4[1] + acc4[2] + acc4[3] + acc4[4] +
                        acc4[5] + acc4[6] + acc4[7];
                    data_C[i * C->column + j + 5] =
                        acc5[0] + acc5[1] + acc5[2] + acc5[3] + acc5[4] +
                        acc5[5] + acc5[6] + acc5[7];
                    data_C[i * C->column + j + 6] =
                        acc6[0] + acc6[1] + acc6[2] + acc6[3] + acc6[4] +
                        acc6[5] + acc6[6] + acc6[7];
                    data_C[i * C->column + j + 7] =
                        acc7[0] + acc7[1] + acc7[2] + acc7[3] + acc7[4] +
                        acc7[5] + acc7[6] + acc7[7];
                }
        }

    return NULL;
}

void overall_mat_mul(const struct matmul_params *params)
{
    const int num_thread = 10;  // cpu cores
    int j;

    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    float *data_B = B->data_ptr;
    // transpose the B
    for (int i = 0; i < B->column; i++)
        for (int j = 0; j < B->row; j++)
            overall_transpose_tmp[i * B->row + j] = data_B[j * B->column + i];

    struct matrix transpose_B = {
        .column = B->row, .row = B->column, .data_ptr = overall_transpose_tmp};

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (C->row / num_thread);
        threads_args[j].end_i = (j + 1) * (C->row / num_thread);
        threads_args[j].A = A;
        threads_args[j].B = &transpose_B;
        threads_args[j].C = C;
        pthread_create(&thread_pool[j], NULL, overall_thread_func,
                       &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}