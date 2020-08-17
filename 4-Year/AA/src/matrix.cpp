#include "matrix.h"

void transpose(float * matrix, int N){
    for(int i = 0; i < N; i++)
        for(int j = i+1; j < N; j++){
            float copy = matrix[j*N + i];
            matrix[j*N + i] = matrix[i*N + j];
            matrix[i*N + j] = copy;
        }
}

void fill_matrix(float * matrix,int N){
    float aux;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++){
            aux = (float)rand() / RAND_MAX;
            matrix[i*N + j] = MIN + aux*(MAX - MIN);
        }
}

void fill_matrix_ones(float * matrix,int N){
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++){
            matrix[i*N + j] = 1;
        }
}

void fill_matrix_ide(float * matrix,int N){
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++){
            if (i == j){
                matrix[i*N + j] = 1.0;
            }
            else{
                matrix[i*N + j] = 0.0;
            }
        }
}

// c fixo; a row wise; b column wise
void dot_product_ijk(float * A,float * B,float * C, int N){
    float x;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++){
            x = 0.0f;
            //#pragma novector
            for(int k = 0; k < N; k++){
                x += A[i*N + k] * B[k*N + j];
            }
            C[i*N+j] = x;
        }
}

void dot_product_ijk_transposed(float * A,float * B,float * C, int N) {
    float x;
    transpose(B,N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			x = 0.0;
            //#pragma novector
			for (int k = 0; k < N; k++) {
                //x += A[i*N + k] * B[k*N + j];
                x += A[i*N + k] * B[j*N + k];
			}
			C[i*N+j] = x;
		}
	}
}

// i-k-j
// a fixo; b e c row wise
void dot_product_ikj(float * A,float * B,float * C, int N){
    float x;
    for(int i = 0; i < N; i++)
        for(int k = 0; k < N; k++){
            x = A[i*N + k];
            //#pragma novector
            for(int j = 0; j < N; j++){
                C[i*N + j] += x * B[k*N + j];
            }
        }
}


// j-k-i
// b fixo, acessos; a e c column wise
void dot_product_jki(float * A,float * B,float * C, int N) {
	float x;
	for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
            x = B[k*N + j];
			for (int i = 0; i < N; i++) {
                C[i*N + j] += A[i*N + k] * x;
			}
		}
	}
}

void dot_product_jki_transposed(float * A,float * B,float * C, int N) {
	float x;

    transpose(A,N);
    transpose(B,N);

	for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
            x = B[k*N + j];
            //#pragma novector
			for (int i = 0; i < N; i++) {
                C[j*N + i] += A[k*N + i] * x;
			}
		}
    }
    transpose(C,N);
}

void dot_product_blocking(float * A,float * B,float * C, int N,int block_size){
    for(int i = 0; i < N; i += block_size)
        for(int j = 0; j < N; j += block_size) 
            for(int k = 0; k < N; k += block_size){
				int maxi = i + block_size;
				for(int ii = i; ii < maxi; ii++){
					int maxj = j + block_size;
					for(int jj = j; jj < maxj; jj++){
						int maxk = k + block_size;
						for(int kk = k; kk < maxk; kk++){
							C[ii*N + jj] += A[ii*N + kk] * B[kk*N + jj];
						}
					}
				}
			}
}

void dot_product_blocking_vectorizable(float * A,float * B,float * C, int N,int block_size){
	transpose(B,N);

    for(unsigned i = 0; i < N; i += block_size)
        for(unsigned j = 0; j < N; j += block_size) 
            for(unsigned k = 0; k < N; k += block_size){
				int maxi = i + block_size;
				for(unsigned ii = i; ii < maxi; ii++){
					int maxj = j + block_size;
					for(unsigned jj = j; jj < maxj; jj++){
						int maxk = k + block_size;
						for(unsigned kk = k; kk < maxk; kk++){
							C[ii*N + jj] += A[ii*N + kk] * B[jj*N + kk];
						}
					}
				}
			}
    //cout << checksum(A,C,N) << endl;
}

void dot_product_blocking_vectorizable_parallel(float * A,float * B,float * C, int N,int block_size){
	transpose(B,N);

    #pragma omp parallel num_threads(24)
    {
        #pragma omp for 
        for(unsigned i = 0; i < N; i += block_size)
            for(unsigned j = 0; j < N; j += block_size) 
                for(unsigned k = 0; k < N; k += block_size){
                    int maxi = i + block_size;
                    for(unsigned ii = i; ii < maxi; ii++){
                        int maxj = j + block_size;
                        for(unsigned jj = j; jj < maxj; jj++){
                            int maxk = k + block_size;
                            for(unsigned kk = k; kk < maxk; kk++){
                                C[ii*N + jj] += A[ii*N + kk] * B[jj*N + kk];
                            }
                        }
                    }
                }
    }
}


void dot_product_blocking_vectorizable_parallel_knl(float * A,float * B,float * C, int N,int block_size){
	transpose(B,N);

    #pragma omp parallel num_threads(128)
    {
        #pragma omp for 
        for(unsigned i = 0; i < N; i += block_size)
            for(unsigned j = 0; j < N; j += block_size) 
                for(unsigned k = 0; k < N; k += block_size){
                    int maxi = i + block_size;
                    for(unsigned ii = i; ii < maxi; ii++){
                        int maxj = j + block_size;
                        for(unsigned jj = j; jj < maxj; jj++){
                            int maxk = k + block_size;
                            for(unsigned kk = k; kk < maxk; kk++){
                                C[ii*N + jj] += A[ii*N + kk] * B[jj*N + kk];
                            }
                        }
                    }
                }
    }
}

void dot_product_blocking_vectorizable_test_knl(float * A,float * B,float * C, int N,int nthreads){
    #pragma omp parallel num_threads(nthreads)
    {
        float x;
        #pragma omp for schedule(static, 2) private(x)
        for(int i = 0; i < N; i++)
            for(int k = 0; k < N; k++){
                x = A[i*N + k];
                for(int j = 0; j < N; j++){
                    C[i*N + j] += x * B[k*N + j];
                }
            }
    }
}

bool checksum(float * A, float * C,int N){
    bool result = true;

    float checksum;
    for(int i=0; i<N; i++){
        checksum = 0.0;
        for(int k = 0; k < N; k++){
            checksum += A[i*N+k];
        }
        for(int j = 0; j < N; j++){
            if(C[i*N + j] != checksum){
                result = false;
                cout << C[i*N + j] - checksum << endl;
            }
        }
    }

    return result;
}

bool verify(float * A,float * C,int N){
    for(int i = 0; i < N ; i++)
        for(int j = 0; j < N; j++)
            if (A[i*N+j] != C[i*N+j])
                return false;
    return true;
}