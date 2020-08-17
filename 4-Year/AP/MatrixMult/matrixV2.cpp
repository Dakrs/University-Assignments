#include <random>
#include <iostream>
#include <omp.h>
#include <sys/time.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <immintrin.h>

#define SIZE 4096

#define TIME_RESOLUTION 1000000

long long unsigned initial_time;
timeval t;

//double * c = new double[SIZE*SIZE];
//double * a = new double[SIZE*SIZE];
//double * b = new double[SIZE*SIZE];
double c[SIZE*SIZE] __attribute__ ((aligned(64)));
double a[SIZE*SIZE] __attribute__ ((aligned(64)));
double b[SIZE*SIZE] __attribute__ ((aligned(64)));
double b1[SIZE*SIZE] __attribute__ ((aligned(64)));


void start (void) {
    gettimeofday(&t, NULL);
    initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

long long unsigned stop (void) {
	gettimeofday(&t, NULL);
	long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

	return final_time - initial_time;
}

void randomBuild(double * a, double * b, double * c){
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      a[i*SIZE + j] = (double)rand() / RAND_MAX;
      c[i*SIZE + j] = 0.0;
    }
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (i == j)
        b[i*SIZE + j] = 1.0;
      else
        b[i*SIZE + j] = 0.0;;
    }
  }
}

void transpose(double * b){
  #pragma omp parallel for
  for(int i = 0; i < SIZE; i++){
    for(int j = i; j < SIZE; j++){
      double temp = b[i*SIZE + j];
      b[i*SIZE + j] = b[j*SIZE + i];
      b[j*SIZE + i] = temp;
    }
  }
}

void print(){
  std::cout << "<<<<<<<<<<<< A >>>>>>>>>>>>" << std::endl;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      std::cout << a[i*SIZE + j] << " : ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "<<<<<<<<<<<< B >>>>>>>>>>>>" << std::endl;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      std::cout << b[i*SIZE + j] << " : ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "<<<<<<<<<<<< C >>>>>>>>>>>>" << std::endl;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      std::cout << c[i*SIZE + j] << " : ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}

void multiplication_blis(double * __restrict__ a,double * __restrict__ b, double * __restrict__ c) {
  long long unsigned tt;
  transpose(b);
    for(int kc = 0; kc < SIZE; kc += 512){
      for(int mc = 0; mc < SIZE; mc += 256){
        #pragma omp parallel for firstprivate(kc,mc) schedule(static, 4)
        for(int nr = 0; nr < SIZE; nr++){
          int max_mr = mc + 256;
          for(int mr = mc; mr < max_mr; mr+=4){
            double res = c[mr*SIZE + nr];
            double res1 = c[(mr+1)*SIZE + nr];
            double res2 = c[(mr+2)*SIZE + nr];
            double res3 = c[(mr+3)*SIZE + nr];
            #pragma omp simd reduction(+:res,res1,res2,res3)
            for(int k = 0; k < 512; k++){
                res += a[mr*SIZE + kc+k] * b[nr*SIZE + (kc+k)];
                res1 += a[(mr+1)*SIZE + kc+k] * b[nr*SIZE + (kc+k)];
                res2 += a[(mr+2)*SIZE + kc+k] * b[nr*SIZE + (kc+k)];
                res3 += a[(mr+3)*SIZE + kc+k] * b[nr*SIZE + (kc+k)];
            }
            c[mr*SIZE + nr] = res;
            c[(mr+1)*SIZE + nr] = res1;
            c[(mr+2)*SIZE + nr] = res2;
            c[(mr+3)*SIZE + nr] = res3;
          }
        }
      }
    }
}

bool test(double * a,  double * c){
  bool flag = false;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (a[i*SIZE + j] != c[i*SIZE + j]){
        flag = true;
      }
    }
  }
  return flag;
}


int main(int argc, char const *argv[]) {

  srand(time(NULL));
  randomBuild(a,b,c);
  //print();
  long long unsigned tt;
  start();
  multiplication_blis(a,b,c);
  tt = stop();
  std::cout << tt << " usecs" << std::endl;
  bool error = test(a,c);
  //print();
  std::cout << "Erros: " << error << std::endl;
  return 0;
}
