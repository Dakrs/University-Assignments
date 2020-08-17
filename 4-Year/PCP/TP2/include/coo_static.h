#include "matrix.h"
#include <ostream>
#include <iostream>
#include <mpi.h>

using namespace std;

class StaticCOO {
    int COLS;
    int ROWS;

    int chunk_size;
    int work_sent;

    public:
        float * matrix;
        float * non_zero_values;
        int * non_zero_rows;
        int * non_zero_cols;
        int zeros;
        float * vector;
        float * result;
        int * chunkdiv;
        int * chunk_count;

        StaticCOO(int n_rows,int n_cols,float p_non_zeros, bool balanced);
        void print();
        void computeZero();
        void setWorkLoad(int chunk_size, int nprocesses);
        void sendWorkLoad(int destination);
        void updateResult(float * buffer);

    private:
        void transform();
};