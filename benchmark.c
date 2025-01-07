#include "matmul.h"

#define A_ROW 640
#define A_COLUMN 12800
#define B_ROW 12800
#define B_COLUMN 640
#define C_ROW 640
#define C_COLUMN 640

float MAT_A[A_ROW * A_COLUMN];
float MAT_B[B_ROW * B_COLUMN];
float MAT_C[C_ROW * C_COLUMN];

int main(int argc, char **argv)
{
    initialize_matrix(MAT_A, A_ROW * A_COLUMN);
    initialize_matrix(MAT_B, B_ROW * B_COLUMN);
    initialize_matrix(MAT_C, C_ROW * C_COLUMN);

    struct matmul_params params;
    params.A.row = A_ROW;
    params.A.column = A_COLUMN;
    params.A.data_ptr = MAT_A;
    params.B.row = B_ROW;
    params.B.column = B_COLUMN;
    params.B.data_ptr = MAT_B;
    params.C.row = C_ROW;
    params.C.column = C_COLUMN;
    params.C.data_ptr = MAT_C;

    enum IMP_TYPE imp = MULTITHREADING;
    evaluate(imp, &params);

    return 0;
}
