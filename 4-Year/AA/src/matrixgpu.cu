#include "matrixgpu.cuh"

cudaEvent_t start, stop, transferTodevice, transferTohost, startTohost;

float datatransfer_time;

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

void measuse_data_transfer_time_to_device(void){
    cudaEventRecord(transferTodevice);

    cudaEventSynchronize(transferTodevice);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, transferTodevice);
    datatransfer_time += milliseconds;
}

void startTransferToHost(void){
    cudaEventRecord(startTohost);
}

void stopTransferToHost(void){
    cudaEventRecord(transferTohost);

    cudaEventSynchronize(transferTohost);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, startTohost, transferTohost);
    datatransfer_time += milliseconds;
}

void startKernelTime (void) {
	cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventCreate(&transferTodevice);
    cudaEventCreate(&transferTohost);
    cudaEventCreate(&startTohost);


	cudaEventRecord(start);
}

void stopKernelTime (void) {
	cudaEventRecord(stop);

	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);

	cout << milliseconds << " ms have elapsed for the CUDA execution" << endl;
}

void checkCUDAError (const char *msg) {
	cudaError_t err = cudaGetLastError();
	if( cudaSuccess != err) {
		cerr << "Cuda error: " << msg << ", " << cudaGetErrorString( err) << endl;
		exit(-1);
	}
}


__global__
void block_dot_product_v1(float * A,float * B,float * C, int N){
    int rowBlock = blockIdx.y*blockDim.y + threadIdx.y;
    int colBlock = blockIdx.x*blockDim.x + threadIdx.x;

    float result = 0;
    __shared__ float sA[TILE_SIZE][TILE_SIZE];
    for(int i = 0; i < N; i += TILE_SIZE){
        sA[threadIdx.y][threadIdx.x] = A[rowBlock*N + i + threadIdx.x];

        __syncthreads();
        for(int k = 0; k < TILE_SIZE; k++){
            result += sA[threadIdx.y][k] * B[(i+k)*N + colBlock];
        }
        __syncthreads();
    }
    C[rowBlock*N + colBlock] = result;
}


__global__
void block_dot_product_v2(float * A,float * B,float * C, int N){
    int rowBlock = blockIdx.y*blockDim.y + threadIdx.y;
    int colBlock = blockIdx.x*blockDim.x + threadIdx.x;

    float result = 0;
    __shared__ float sA[TILE_SIZE][TILE_SIZE];
    __shared__ float sB[TILE_SIZE][TILE_SIZE];
    for(int i = 0; i < N; i += TILE_SIZE){
        sA[threadIdx.y][threadIdx.x] = A[rowBlock*N + i + threadIdx.x];
        sB[threadIdx.y][threadIdx.x] = B[(i+threadIdx.y)*N + colBlock];
        __syncthreads();
        for(int k = 0; k < TILE_SIZE; k++){
            result += sA[threadIdx.y][k] * sB[k][threadIdx.x];
        }
        __syncthreads();
    }
    C[rowBlock*N + colBlock] = result;
}

__global__
void block_dot_product(float * A,float * B,float * C, int N){
    int row = blockIdx.y*blockDim.y + threadIdx.y;
    int column = blockIdx.x*blockDim.x + threadIdx.x;

    float result = 0.0;

    for(int i = 0; i < N; i++)
        result += A[row*N + i] * B[i*N + column];
    C[row*N + column] = result;
}

void dot_product_gpu(float * A,float * B,float * C, int N){
    float * matA, * matB, * output;
    int bytes = N*N*sizeof(float);

    datatransfer_time = 0.0;

    cudaMalloc((void**) &matA, bytes);
    cudaMalloc((void**) &matB, bytes);
    cudaMalloc((void**) &output, bytes);

    checkCUDAError("mem allocation");

    startKernelTime();

    cudaMemcpy(matA,A,bytes,cudaMemcpyHostToDevice);
    cudaMemcpy(matB,B,bytes,cudaMemcpyHostToDevice);
    cudaMemset(output,0.0,bytes);

    measuse_data_transfer_time_to_device();

    checkCUDAError("mem set");

    dim3 threadsPerBlock(TILE_SIZE,TILE_SIZE);
    dim3 numBlocks(N / threadsPerBlock.x, N / threadsPerBlock.y);

    block_dot_product_v2 <<< numBlocks , threadsPerBlock >>> (matA,matB,output,N);

    cudaThreadSynchronize();

    startTransferToHost();

    cudaMemcpy(C,output,bytes,cudaMemcpyDeviceToHost);

    stopTransferToHost();

    stopKernelTime();

    cout << "Data Transfer time: " << datatransfer_time << endl;

    cudaFree(matA);
    cudaFree(matB);
    cudaFree(output);

    //bool check = checksum(A,C,N);
    //cout << check << endl;
}