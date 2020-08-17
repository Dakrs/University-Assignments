#include <time.h>
#include <math.h>
#include <cstdlib>
#include <omp.h>
#include <ostream>
#include <iostream>

using namespace std;

#define MAX 10000
#define MIN 0

void fill_matrix(float * matrix, int N);
void fill_matrix_ones(float * matrix, int N);
void fill_matrix_ide(float * matrix,int N);
void dot_product_ijk(float * A,float * B,float * C, int N);
void dot_product_ikj(float * A,float * B,float * C, int N);
void dot_product_ijk_transposed(float * A,float * B,float * C, int N);
void dot_product_jki(float * A,float * B,float * C, int N);
void dot_product_jki_transposed(float * A,float * B,float * C, int N);


void dot_product_blocking(float * A,float * B,float * C, int N,int block_size);
void dot_product_blocking_vectorizable(float * A,float * B,float * C, int N,int block_size);
void dot_product_blocking_vectorizable_parallel(float * A,float * B,float * C, int N,int block_size);
void dot_product_blocking_vectorizable_parallel_knl(float * A,float * B,float * C, int N,int block_size);
void dot_product_blocking_vectorizable_test_knl(float * A,float * B,float * C, int N,int nthreads);

void transpose(float * matrix, int N);
bool checksum(float * A, float * C,int N);
bool verify(float * A,float * C,int N);


