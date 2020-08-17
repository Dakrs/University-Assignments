#include "matrix.h"
#include "yale_sparse.h"
#include "coo_sparse.h"
#include "my_papi.h"
#include "measures.h"

float * matrix;
float * nonn_zero_values;
int * nonn_zero_acu;
int * nonn_zero_pos;
int * nonn_zero_row;
int * nonn_zero_col;
float * vvector;
float * rresult;
int zeros;

int ROWS;
int COLS;

void malloc_matrix_vec(int n_rows,int n_cols,float p_non_zeros){
    int n_elem = n_rows * n_cols;
    int n_zeros = ceil(n_cols*p_non_zeros) * n_rows;

    matrix = malloc(sizeof(float)*n_elem);
    nonn_zero_values = malloc(sizeof(float) * n_zeros);
    nonn_zero_acu = malloc(sizeof(int) * (n_rows + 1));
    nonn_zero_pos = malloc(sizeof(int) * n_zeros);

    vvector = malloc(sizeof(float) * n_cols);
    rresult = malloc(sizeof(float) * n_rows);

    rand_sparse_vec_balanced(n_rows,n_cols,matrix,p_non_zeros);
    //rand_sparse_vec(n_rows,n_cols,matrix,p_non_zeros);
    rand_vector(n_cols,vvector);

    for(int i = 0; i < n_rows; i++)
        rresult[i] = 0.0;

    ROWS = n_rows;
    COLS = n_cols;

    build_yale_vec(n_rows,n_cols,matrix,nonn_zero_values,nonn_zero_acu,nonn_zero_pos);

    zeros = n_zeros;
}

void malloc_coomatrix(int n_rows,int n_cols,float p_non_zeros){
  int n_elem = n_rows * n_cols;
  int n_zeros = round(n_elem * p_non_zeros);

  matrix = malloc(sizeof(float)*n_elem);
  nonn_zero_values = malloc(sizeof(float) * n_zeros);
  nonn_zero_row = malloc(sizeof(int) * n_zeros);
  nonn_zero_col = malloc(sizeof(int) * n_zeros);

  vvector = malloc(sizeof(float) * n_cols);
  rresult = malloc(sizeof(float) * n_rows);

  rand_sparse_vec(n_rows,n_cols,matrix,p_non_zeros);
  rand_vector(n_cols,vvector);

  for(int i = 0; i < n_rows; i++)
      rresult[i] = 0.0;

  ROWS = n_rows;
  COLS = n_cols;

  build_coo_mat(n_rows,n_cols,matrix,nonn_zero_values, nonn_zero_row, nonn_zero_col);

  zeros = n_zeros;
}


void free_matrix(){
  free(nonn_zero_values);
  free(matrix);
  free(nonn_zero_acu);
  free(nonn_zero_pos);
  free(vvector);
  free(rresult);
}

int main(int argc, char const *argv[])
{
    //srand(time(NULL));
    srand(1);

    int i = argc >= 2 ? atoi(argv[1]) : 1;
    float per = argc >= 3 ? atof(argv[2]) : 0.2;
    int size = argc >= 4 ? atoi(argv[3]) : 1024;
    int n_threads = argc >= 5 ? atoi(argv[4]) : 4;
    int repetitions = argc >= 6 ? atoi(argv[5]) : 1;
    double time;

    int rep = 0;
    double * time_measure, average_time;

    initPapi(1);
    createEventSet();

    switch (i) {
      case 0:
        //setMemoryStats(repetitions);
        //setLoadStoreCounter(repetitions);
        setFloatCounter(repetitions);
        time_measure = malloc(sizeof(double)*repetitions);
        malloc_matrix_vec(size,size,per);
        while(rep < repetitions){
          clearCache();
          time = omp_get_wtime();
          start();
          sparse_vec_mul_seq(ROWS,COLS,nonn_zero_values,nonn_zero_acu,nonn_zero_pos,vvector,rresult);
          stop();
          time = omp_get_wtime()-time;
          time_measure[rep] = time;
          rep++;
        }
        //getMemStats();
        //getLoadStoreResults();
        getFloatInstResults();
        average_time = meanKBestDouble(time_measure,repetitions, 5,0);
        printf("Seq - Array - KBest 5 - Time=%f\n",average_time);
        break;
      case 1:
        time_measure = malloc(sizeof(double)*repetitions);
        malloc_matrix_vec(size,size,per);
        printf("Rep - %d\n",repetitions);
        while(rep < repetitions){
          clearCache();
          time = omp_get_wtime();
          sparse_vec_mul_parallel(ROWS,COLS,nonn_zero_values,nonn_zero_acu,nonn_zero_pos,vvector,rresult,n_threads);
          time = omp_get_wtime()-time;
          time_measure[rep] = time;
          rep++;
        }
        average_time = meanKBestDouble(time_measure,repetitions, 5,0);
        printf("Par - Array - KBest 5 - Time=%f\n",average_time);
        break;
      case 11:
        time_measure = malloc(sizeof(double)*repetitions);
        malloc_matrix_vec(size,size,per);
        printf("Rep - %d\n",repetitions);
        while(rep < repetitions){
          clearCache();
          time = omp_get_wtime();
          sparse_vec_mul_parallel_initial(ROWS,COLS,nonn_zero_values,nonn_zero_acu,nonn_zero_pos,vvector,rresult,n_threads);
          time = omp_get_wtime()-time;
          time_measure[rep] = time;
          rep++;
        }
        average_time = meanKBestDouble(time_measure,repetitions, 5,0);
        printf("ParInitial - Array - KBest 5 - Time=%f\n",average_time);
        break;
      case 2:
        malloc_coomatrix(size,size,per);
          time = omp_get_wtime();
          sparse_coo_mul_seq(ROWS,COLS,zeros,nonn_zero_values,nonn_zero_row,nonn_zero_col,vvector,rresult);
        printf("Seq - COOMat - KBest 5 - Time=%f\n", omp_get_wtime()-time);
        break;
        
      case 3:
        malloc_coomatrix(size,size,per);
        time = omp_get_wtime();
        sparse_coo_mul_parallel(ROWS,COLS,zeros,nonn_zero_values,nonn_zero_row,nonn_zero_col,vvector,rresult,n_threads);
        printf("Par - COOMat - KBest 5 - Time=%f\n", omp_get_wtime()-time);
        break;

      default:
        break;
    }

    
    float * result2 = malloc(sizeof(float)*ROWS);

    time = omp_get_wtime();
    mult_mat(ROWS,COLS,matrix,vvector,result2);
    printf("Time=%f\n", omp_get_wtime()-time);

    int c = 0;

    for (int c = 0; c < ROWS; c++)
    {
      if (rresult[c] != result2[c]){
        printf("Oh shit, %i :: %f - %f\n",c, rresult[c], result2[c]);
        break;
      }

    }

    free_matrix();

    return 0;
}
