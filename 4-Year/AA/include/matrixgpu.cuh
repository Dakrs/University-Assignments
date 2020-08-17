#include <ostream>
#include <iostream>
#include <stdio.h>
#include "matrix.h"

#define TILE_SIZE 16
#define NUM_THREAD_BLOCK TILE_SIZE*TILE_SIZE

using namespace std;

void dot_product_gpu(float * A,float * B,float * C, int N);