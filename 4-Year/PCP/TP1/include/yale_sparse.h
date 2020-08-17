#include <omp.h>
#include <stdio.h>

/**
 * NON_ZERO_VALUES -> array with all the non zero values of the matrix
 * NON_ZERO_ACU -> number o non zeros acumulates. NON_ZERO_ACO[i+1] - NON_ZERO_ACO[i] gives the number of non zeros at row i
 * NON_ZERO_POS -> column number of each element.
 *
 * */

void build_yale_vec(
    int ROWS,
    int COLS,
    float * mat,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos
);

//sparse_matrix_mul_parallel
void sparse_vec_mul_parallel(
    int ROWS,
    int COLS,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos,
    float * vector,
    float * res,
    int n_threads
);

void sparse_vec_mul_parallel_initial(
    int ROWS,
    int COLS,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos,
    float * vector,
    float * res,
    int n_threads
);

void sparse_vec_mul_seq(
  int ROWS,
  int COLS,
  float * nonn_zero_values,
  int * nonn_zero_acu,
  int * nonn_zero_pos,
  float * vector,
  float * res
);

void sparse_vec_mul_seq_cache_v1(
  int ROWS,
  int COLS,
  float * nonn_zero_values,
  int * nonn_zero_acu,
  int * nonn_zero_pos,
  float * vector,
  float * res
);

void sparse_vec_mul_seq_cache_v2(
  int ROWS,
  int COLS,
  float * nonn_zero_values,
  int * nonn_zero_acu,
  int * nonn_zero_pos,
  float * vector,
  float * res
);
