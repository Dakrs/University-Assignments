#include "matrix.h"
#include <ostream>
#include <iostream>
#include <mpi.h>

using namespace std;

class CRS {
    int COLS;
    int ROWS;
    int zeros;

    public:
        float * matrix;
        float * non_zero_values;
        int * non_zero_acu;
        int * non_zero_pos;
        float * vector;
        float * result;

        CRS(int n_rows,int n_cols,float p_non_zeros,bool balanced);
        void print();

    private:
        void transform();
};