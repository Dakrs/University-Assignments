#include "matrix.h"
#include <ostream>
#include <iostream>
#include <mpi.h>
#include "crs.h"
#include <cstdlib>

using namespace std;

double starting_time;
double ending_time;

double Tcomm;
double Tfree;
double Tcomp;

void setTimer(){
    Tcomm = 0.0;
    Tfree = 0.0;
    Tcomp = 0.0;
    starting_time = MPI_Wtime();
}

void getMessage(MPI_Comm comm){
    MPI_Status status;
    double init = MPI_Wtime();
    MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,comm,&status);
    Tfree += MPI_Wtime() - init;
}

void endTimer(int id){
    ending_time = MPI_Wtime();

    double total_time_exectuted = ending_time - starting_time;
    double per_comm = (Tcomm / total_time_exectuted) * 100.0;
    double per_free = (Tfree / total_time_exectuted) * 100.0;
    double per_comp = (Tcomp / total_time_exectuted) * 100.0;

    printf("%d;%5.5lf;%3.2lf;%3.2lf;%3.2lf;\n",id,total_time_exectuted,per_comm,per_free,per_comp);
}

void endTimerMaster(){
    ending_time = MPI_Wtime();

    double total_time_exectuted = ending_time - starting_time;
    double per_comm = (Tcomm / total_time_exectuted) * 100.0;
    double per_free = (Tfree / total_time_exectuted) * 100.0;
    double per_comp = (Tcomp / total_time_exectuted) * 100.0;

    printf("MASTER;%5.5lf;%3.2lf;%3.2lf;%3.2lf;\n",total_time_exectuted,per_comm,per_free,per_comp);
}

int main(int argc, char *argv[])
{
    srand(3);
    int nprocesses; 
    int myrank; 

    MPI_Status status;
    MPI_Init(&argc, &argv); 
 
    MPI_Comm_size(MPI_COMM_WORLD, &nprocesses); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    double time_init;

    int SIZE = argc > 1 ? atoi(argv[1]) : 4096;
    int bcd = argc > 2 ? atoi(argv[2]) : 1;
    int nthreads = argc > 3 ? atoi(argv[3]) : 4;

    bool balanced = bcd == 1;

    int ROWS = SIZE;
    int COLS = SIZE;

    int chunk_size = ROWS / nprocesses;
    
    int * acu_index = new int[nprocesses];
    int * acu_dipls = new int[nprocesses];


    for(int i = 0; i < nprocesses; i++){
        acu_index[i] = chunk_size;
        acu_dipls[i] = i*chunk_size;
    }
    
    if (myrank == 0){
        CRS * server = new CRS(ROWS,COLS,0.2,balanced);
        setTimer();

        for(int i = 0; i < ROWS; i++){
            int k = server->non_zero_acu[i];
            int limit = server->non_zero_acu[i+1];
            float result = 0;

            for(; k < limit; k++){
                result += server->non_zero_values[k] * server->vector[server->non_zero_pos[k]];
            }
            server->result[i] = result;
        }
        endTimerMaster();

        float * result2 = new float[ROWS];
        mult_mat(ROWS,COLS,server->matrix,server->vector,result2);
        for (int c = 0; c < ROWS; c++){
            if (server->result[c] != result2[c]){
                cout << "error at " << c << endl;
                break;
            }
        }
    }
    MPI_Finalize();


    return 0;
}