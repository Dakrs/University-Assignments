#include "matrix.h"
#include <ostream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/time.h>
#include "measures.h"

#ifdef D_GPU
#include "matrixgpu.cuh"
#else
#include <omp.h>
#include "mypapi.h"
#endif

using namespace std;

#define USING_MAIN_1
#define TIME_RESOLUTION 1000000
#define SIZE 1024

double clearcache [30000000];

long long unsigned initial_time;
timeval t;


void clearCache (void) {
	for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
}

void start (void) {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

void printResults (long long unsigned tt) {
	cout << "Execution time: " << tt << " usecs" << endl;
}

long long unsigned stop (void) {
	gettimeofday(&t, NULL);
	long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

	return final_time - initial_time;
}

#ifdef USING_MAIN_1
int main(int argc, char *argv[]){

    //srand(time(NULL));
    srand(1);

    int N = 128;

    float * A = (float*) _mm_malloc(N*N*sizeof(float), 64); 
    float * B = (float*) _mm_malloc(N*N*sizeof(float), 64); 
    float * C = (float*) _mm_malloc(N*N*sizeof(float), 64); 
    float * C2 = (float*) _mm_malloc(N*N*sizeof(float), 64); 

    double time;

    int rep = 3;
    int k = 3;
    int maxRep = 8;
    double * time_measure, average_time;

    fill_matrix(A,N);
    fill_matrix_ones(B,N);

    

    time_measure = (double*)malloc(sizeof(double)*rep);
    int t = 0;
    int runs = 0;
    int flag = 0;

    bool measuringpapi = false, measuringtime = false;
    bool roofline = true;

    if(measuringpapi){
        initPapi();
        createEventSet();
        setMissRatesL1(rep);

        cout << "começou" << endl;


        while(runs < rep)
        {
            clearCache();
            startPAPI();
            dot_product_ijk(A,B,C,N);
            stopPAPI();
            runs++;
            cout << "run" << endl;
        }
        cout << "end" << endl;
        getMissRatesL1();

    }
    else if (measuringtime){
        cout << "começou medição de tempo" << endl;
        while(runs < maxRep && flag==0)
        {
            clearCache();
            start();
            dot_product_jki_transposed(A,B,C,N);
            time = stop();
            time_measure[runs] = time;

            if(runs >= k-1){
                if(hasConverged(time_measure,runs,3,0.05)){
                    average_time = meanKBestDouble(time_measure,runs,3,0);
                    flag = 1;
                }
            }

            runs++;
            cout << "run" << endl;
        }
    
        if(flag == 0){
            average_time = meanKBestDouble(time_measure,runs,3,0);
        }
        
        printf("%d;%lld\n",N,average_time);
    }
    else if(roofline){
        dot_product_ijk(A,B,C,N);
        dot_product_ijk_transposed(A,B,C,N);
        dot_product_ikj(A,B,C,N);
        dot_product_jki(A,B,C,N);
        dot_product_jki_transposed(A,B,C,N);
    }


    return 0;
}
#else
int main(int argc, char *argv[]){

    //srand(time(NULL));
    srand(1);

    int N = 2048;

    float * A = new float[N*N];
    float * B = new float[N*N];
    float * C = new float[N*N];
    float * C2 = new float[N*N];

    double time;

    int rep = 8;
    double * time_measure, average_time;

    fill_matrix(A,N);
    //fill_matrix_ones(B,N);
    fill_matrix_ide(B,N);

    #ifdef D_GPU
    int runs = 0;

    long long unsigned tt;

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            C[i*N + j] = 0.0;

    cout << "começou" << endl;

    
    while(runs < rep){
        clearCache();
        start();
        dot_product_gpu(A,B,C,N);
        tt = stop();
        printResults(tt);
        runs++;
    }


    #else
    
    
    time_measure = (double*)malloc(sizeof(double)*rep);
    int t = 0;

    int runs = 0;

    long long unsigned tt;

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            C[i*N + j] = 0.0;

    cout << "começou" << endl;

    
    while(runs < rep){
        clearCache();
        start();
        dot_product_blocking_vectorizable_parallel_knl(A,B,C,N,32);
        tt = stop();
        printResults(tt);
        runs++;
    }
    /**
    int nthreads = 64;
    cout << nthreads << endl;
    while(runs < rep){
        clearCache();
        start();
        dot_product_blocking_vectorizable_test_knl(A,B,C,N,nthreads);
        tt = stop();
        printResults(tt);
        runs++;
    }
    runs = 0;
    nthreads = 128;
    cout << nthreads << endl;
    while(runs < rep){
        clearCache();
        start();
        dot_product_blocking_vectorizable_test_knl(A,B,C,N,nthreads);
        tt = stop();
        printResults(tt);
        runs++;
    }

    runs = 0;
    nthreads = 256;
    cout << nthreads << endl;
    while(runs < rep){
        clearCache();
        start();
        dot_product_blocking_vectorizable_test_knl(A,B,C,N,nthreads);
        tt = stop();
        printResults(tt);
        runs++;
    }*/

    #endif

    /**
    while(n < rep){
        //time = omp_get_wtime();

        //singleDotProduct(c,a,b,SIZE);
        //time = omp_get_wtime()-time;
        //time_measure[rep] = time;
        n++;
    }
    //average_time = meanKBestDouble(time_measure,rep,3,0);
    //printf("KBest 3 - Time=%f\n",average_time);

    dot_product_ijk(A,B,C,N);
    */

    return 0;
}
#endif