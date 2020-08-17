#include <stdio.h>
#include <stdlib.h>

void build_coo_mat(
    int ROWS,
    int COLS,
    float * mat,
    float * nonn_zero_values,
    int * nonn_zero_row, 
    int * nonn_zero_col
);

void sparse_coo_mul_parallel(
    int ROWS,
    int COLS,
    int nz,
    float * nonn_zero_values,
    int * nonn_zero_row,
    int * nonn_zero_col,
    float * vector,
    float * res,
    int n_threads
);

void sparse_coo_mul_seq(
    int ROWS,
    int COLS,
    int nz,
    float * nonn_zero_values,
    int * nonn_zero_row,
    int * nonn_zero_col,
    float * vector,
    float * res
);