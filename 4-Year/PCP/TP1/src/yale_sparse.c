#include "yale_sparse.h"

void build_yale_vec(
    int ROWS,
    int COLS,
    float * mat,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos
)
{
  int n_nonzeros = 0;
  nonn_zero_acu[0] = 0;

  for(int i = 0; i < ROWS; i++){
      int n_nonzeros_row = 0;
      for (int j = 0; j < COLS; j++)
      {
          if (mat[i*COLS + j] != 0.0){
              nonn_zero_values[n_nonzeros] = mat[i*COLS + j];
              nonn_zero_pos[n_nonzeros] = j;
              n_nonzeros++;
              n_nonzeros_row++;
          }
      }
      nonn_zero_acu[i+1] = nonn_zero_acu[i] + n_nonzeros_row;
  }
}

void sparse_vec_mul_parallel(
    int ROWS,
    int COLS,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos,
    float * vector,
    float * res,
    int n_threads
)
{
    #pragma omp parallel num_threads(n_threads)
    {
      #pragma omp single
      {
        for(int i = 0; i < ROWS; i+= 64){
          #pragma omp task firstprivate(i)
          {
            int max = i + 64;
            for(; i < max; i++){
              int k = nonn_zero_acu[i];
              int limit = nonn_zero_acu[i+1];
              float result = 0.0;

              for(; k < limit; k++){
                  result += nonn_zero_values[k] * vector[nonn_zero_pos[k]];
              }

              res[i] = result;
            }
          }
        }
        #pragma omp taskwait
      }
    }
}

void sparse_vec_mul_seq(
    int ROWS,
    int COLS,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos,
    float * vector,
    float * res
)
{
    for(int i = 0; i < ROWS; i++){
        int k = nonn_zero_acu[i];
        int limit = nonn_zero_acu[i+1];
        float result = 0;

        for(; k < limit; k++){
            result += nonn_zero_values[k] * vector[nonn_zero_pos[k]];
        }
        res[i] = result;
    }
}


void sparse_vec_mul_seq_cache_v1(
    int ROWS,
    int COLS,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos,
    float * vector,
    float * res
)
{
    for(int i = 0; i < ROWS; i++){
        int k = nonn_zero_acu[i];
        int limit = nonn_zero_acu[i+1];
        float result = 0;

        for(; k < limit; k++){
            result += nonn_zero_values[k];
        }
        res[i] = result;
    }
}

void sparse_vec_mul_seq_cache_v2(
    int ROWS,
    int COLS,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos,
    float * vector,
    float * res
)
{
    for(int i = 0; i < ROWS; i++){
        int k = nonn_zero_acu[i];
        int limit = nonn_zero_acu[i+1];
        float result = 0;

        for(; k < limit; k++){
            result += vector[nonn_zero_pos[k]];
        }
        res[i] = result;
    }
}

void sparse_vec_mul_parallel_initial(
    int ROWS,
    int COLS,
    float * nonn_zero_values,
    int * nonn_zero_acu,
    int * nonn_zero_pos,
    float * vector,
    float * res,
    int n_threads
)
{
  #pragma omp parallel for num_threads(n_threads)
    for(int i = 0; i < ROWS; i++){
        int k = nonn_zero_acu[i];
        int limit = nonn_zero_acu[i+1];
        float result = 0.0;

        for(; k < limit; k++){
            result += nonn_zero_values[k] * vector[nonn_zero_pos[k]];
        }

        res[i] = result;
    }
}