#include "matrix.h"
#include <ostream>
#include <iostream>
#include <mpi.h>
#include "crs.h"
#include <cstdlib>
#include <omp.h>

#define TASK_SIZE 64

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
    /**
    if (myrank == 0){
        CRS * server = new CRS(ROWS,COLS,0.2,balanced);
        setTimer();

        #pragma omp parallel for num_threads(nthreads)
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
    }*/

    if (myrank == 0){
        CRS * server = new CRS(ROWS,COLS,0.2,balanced);

        int * values_index = new int[nprocesses];
        int * values_count = new int[nprocesses];

        int index = 0;
        for(int i = 0; i < nprocesses; i++){
            values_count[i] = server->non_zero_acu[(i+1)*chunk_size] - server->non_zero_acu[i*chunk_size];
            values_index[i] = index;
            index += server->non_zero_acu[(i+1)*chunk_size] - server->non_zero_acu[i*chunk_size];
        }

        MPI_Barrier(MPI_COMM_WORLD);

        setTimer();

        MPI_Bcast(server->vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Bcast(server->non_zero_acu,ROWS+1,MPI_INT,0,MPI_COMM_WORLD);

        for(int i = 1; i < nprocesses; i++){
            MPI_Send(values_count+i,1,MPI_INT,i,0,MPI_COMM_WORLD);
        }

        float * receiving_work_values = new float[values_count[0]];
        int * receiving_work_pos = new int[values_count[0]];

        MPI_Scatterv(server->non_zero_values,values_count,values_index,MPI_FLOAT,receiving_work_values,values_count[0],MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Scatterv(server->non_zero_pos,values_count,values_index,MPI_INT,receiving_work_pos,values_count[0],MPI_INT,0,MPI_COMM_WORLD);
    

        int offset = chunk_size * myrank;
        
        /**
        #pragma omp parallel num_threads(nthreads)
        {
            #pragma omp single
            {
                for(int i = 0; i < chunk_size; i+= TASK_SIZE){
                    #pragma omp task firstprivate(i)
                    {
                        int max = i + TASK_SIZE;
                        for(; i < max; i++){
                            int k = server->non_zero_acu[i+offset] - server->non_zero_acu[offset];
                            int limit = server->non_zero_acu[i+1+offset] - server->non_zero_acu[offset];
                            float result = 0.0;

                            for(; k < limit; k++){
                                result += receiving_work_values[k] * server->vector[receiving_work_pos[k]];
                            }

                            server->result[i] = result;
                        }
                    }
                }
                #pragma omp taskwait
            }
        }*/
        #pragma omp parallel for num_threads(nthreads)
        for(int i = 0; i < chunk_size; i++){
            int k = server->non_zero_acu[i+offset] - server->non_zero_acu[offset];
            int limit = server->non_zero_acu[i+1+offset] - server->non_zero_acu[offset];
            float result = 0.0;

            for(; k < limit; k++){
                result += receiving_work_values[k] * server->vector[receiving_work_pos[k]];
            }

            server->result[i] = result;
        }

        MPI_Gatherv(server->result,chunk_size,MPI_FLOAT,server->result,acu_index,acu_dipls,MPI_FLOAT,0,MPI_COMM_WORLD);

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
    else{
        float * vector = new float[COLS];
        int * non_zero_acu = new int[ROWS+1];
        float * sending_buffer = new float[chunk_size];

        MPI_Barrier(MPI_COMM_WORLD);

        setTimer();
        
        MPI_Bcast(vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Bcast(non_zero_acu,ROWS+1,MPI_INT,0,MPI_COMM_WORLD);

        int number_to_use;
        MPI_Recv(&number_to_use,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);

        float * receiving_work_values = new float[number_to_use];
        int * receiving_work_pos = new int[number_to_use];

        MPI_Scatterv(NULL,NULL,NULL,MPI_FLOAT,receiving_work_values,number_to_use,MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Scatterv(NULL,NULL,NULL,MPI_INT,receiving_work_pos,number_to_use,MPI_INT,0,MPI_COMM_WORLD);

        int offset = chunk_size * myrank;

        /**
        #pragma omp parallel num_threads(nthreads)
        {
            #pragma omp single
            {
                for(int i = 0; i < chunk_size; i+= TASK_SIZE){
                    #pragma omp task firstprivate(i)
                    {
                        int max = i + TASK_SIZE;
                        for(; i < max; i++){
                            int k = non_zero_acu[i+offset] - non_zero_acu[offset];
                            int limit = non_zero_acu[i+1+offset] - non_zero_acu[offset];
                            float result = 0.0;

                            for(; k < limit; k++){
                                result += receiving_work_values[k] * vector[receiving_work_pos[k]];
                            }

                            sending_buffer[i] = result;
                        }
                    }
                }
                #pragma omp taskwait
            }
        }*/
        
        #pragma omp parallel for num_threads(nthreads)
        for(int i = 0; i < chunk_size; i++){
            int k = non_zero_acu[i+offset] - non_zero_acu[offset];
            int limit = non_zero_acu[i+1+offset] - non_zero_acu[offset];
            float result = 0.0;

            for(; k < limit; k++){
                result += receiving_work_values[k] * vector[receiving_work_pos[k]];
            }

            sending_buffer[i] = result;
        }

        MPI_Gatherv(sending_buffer,chunk_size,MPI_FLOAT,NULL,NULL,NULL,MPI_FLOAT,0,MPI_COMM_WORLD);

        endTimer(myrank);
    }
    MPI_Finalize();


    return 0;
}