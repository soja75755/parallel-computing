struct matrix {
    int row;
    int column;
    float *data_ptr;
};

struct thread_args {
    const struct matrix *A;
    const struct matrix *B;
    const struct matrix *C;
    int start_i, end_i, blk_size;
};

struct optimization_params {
    int blk_size;
    int num_thread;
};

struct matmul_params {
    struct matrix A, B, C;
    struct optimization_params opt_params;
};

// utils
enum IMP_TYPE {
    NAIVE,
    UNROLLING,
    REORDERING,
    TILING,
    MULTITHREADING,
    SIMD,
    OVERALL
};
void initialize_matrix(float *M, int size);
void check_matrices(const struct matrix *A,
                    const struct matrix *B,
                    const struct matrix *C);
void evaluate(enum IMP_TYPE type, const struct matmul_params *params);

// implementaion
void naive_mat_mul(const struct matmul_params *params);
void unrolling_mat_mul(const struct matmul_params *params);
void reordering_mat_mul(const struct matmul_params *params);
void tiling_mat_mul(const struct matmul_params *params);
void multithreading_mat_mul(const struct matmul_params *params);
void simd_mat_mul(const struct matmul_params *params);
void overall_mat_mul(const struct matmul_params *params);