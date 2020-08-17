#include "coo_sparse.h"

void build_coo_mat(
    int ROWS,
    int COLS,
    float * mat,
    float * nonn_zero_values,
    int * nonn_zero_row,
    int * nonn_zero_col
)
{
  int n = 0;
  float val = 0.0;

  for(int i=0; i < ROWS; i++){
    for(int j=0; j < COLS; j++){
        val = mat[i * COLS + j];
        if(val != 0.0){
            nonn_zero_row[n] = i;
            nonn_zero_col[n] = j;
            nonn_zero_values[n] = val;
            n++;
        }
    }
  }
}

void sparse_coo_mul_seq(
    int ROWS,
    int COLS,
    int nz,
    float * nonn_zero_values,
    int * nonn_zero_row,
    int * nonn_zero_col,
    float * vector,
    float * res
)
{
    for(int i = 0; i < nz; i++){
        res[nonn_zero_row[i]] += vector[nonn_zero_col[i]] * nonn_zero_values[i];
    }
}

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
)
{
    #pragma omp parallel num_threads(n_threads)
    {
        float priv[ROWS];
        for(int i=0; i<ROWS; i++)
            priv[i] = 0.0;
        #pragma omp for
        for(int i = 0; i < nz; i++){
            priv[nonn_zero_row[i]] += vector[nonn_zero_col[i]] * nonn_zero_values[i];
        }
        #pragma omp critical
        for(int i=0; i < ROWS; i++){
            res[i] += priv[i];
        }
    }
}
