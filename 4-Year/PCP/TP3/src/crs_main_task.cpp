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
    double time_aux;


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

        time_init = MPI_Wtime();
        MPI_Bcast(server->vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Bcast(server->non_zero_acu,ROWS+1,MPI_INT,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - time_init;

        time_init = MPI_Wtime();
        for(int i = 1; i < nprocesses; i++){
            MPI_Send(server->non_zero_values + values_index[i] ,values_count[i],MPI_FLOAT,i,0,MPI_COMM_WORLD);
            MPI_Send(server->non_zero_pos + values_index[i] ,values_count[i],MPI_INT,i,0,MPI_COMM_WORLD);
        }
        Tcomm += MPI_Wtime() - time_init;

        int offset = chunk_size * myrank;

        time_init = MPI_Wtime();
        #pragma omp parallel num_threads(nthreads)
        {
            #pragma omp single
            {
                for(int i = 0; i < chunk_size; i+= TASK_SIZE){
                    #pragma omp task firstprivate(i)
                    {
                        int max = i + TASK_SIZE;
                        for(; i < max; i++){
                            int k = server->non_zero_acu[i];
                            int limit = server->non_zero_acu[i+1];
                            float result = 0.0;

                            for(; k < limit; k++){
                                result += server->non_zero_values[k] * server->vector[server->non_zero_pos[k]];
                            }

                            server->result[i] = result;
                        }
                    }
                }
                #pragma omp taskwait
            }
        }
        Tcomp += MPI_Wtime() - time_init;

        for(int i = 1; i < nprocesses; i++){
            time_init = MPI_Wtime();
            MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            int source = status.MPI_SOURCE;
            Tfree += MPI_Wtime() - time_init;

            time_init = MPI_Wtime();
            MPI_Recv(server->result + source*chunk_size,chunk_size,MPI_FLOAT,source,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            Tcomm += MPI_Wtime() - time_init;
        }

        time_init = MPI_Wtime();
        MPI_Barrier(MPI_COMM_WORLD);
        Tfree += MPI_Wtime() - time_init;

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

        int bytes;
        MPI_Pack_size(chunk_size, MPI_FLOAT,MPI_COMM_WORLD, &bytes );
        int bufsize = bytes + MPI_BSEND_OVERHEAD;
        float * buffer = new float[bufsize];
        MPI_Buffer_attach( buffer, bufsize );


        MPI_Barrier(MPI_COMM_WORLD);

        setTimer();

        time_aux = MPI_Wtime();
        time_init = MPI_Wtime();
        MPI_Bcast(vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Bcast(non_zero_acu,ROWS+1,MPI_INT,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - time_init;

        time_init = MPI_Wtime();
        MPI_Probe(0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        Tfree += MPI_Wtime() - time_init;

        int number_to_use;

        MPI_Get_count(&status, MPI_INT, &number_to_use);

        float * receiving_work_values = new float[number_to_use];
        int * receiving_work_pos = new int[number_to_use];

        time_init = MPI_Wtime();
        MPI_Recv(receiving_work_values,number_to_use,MPI_FLOAT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        Tcomm += MPI_Wtime() - time_init;

        getMessage(MPI_COMM_WORLD);

        time_init = MPI_Wtime();
        MPI_Recv(receiving_work_pos,number_to_use,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        Tcomm += MPI_Wtime() - time_init;

        int offset = chunk_size * myrank;

        time_init = MPI_Wtime();
        int stop = chunk_size+offset;

        #pragma omp parallel num_threads(nthreads)
        {
            #pragma omp single
            {
                for(int i = offset; i < stop; i+= TASK_SIZE){
                    #pragma omp task firstprivate(i)
                    {
                        int max = i + TASK_SIZE;
                        for(; i < max; i++){
                            int k = non_zero_acu[i] - non_zero_acu[offset];
                            int limit = non_zero_acu[i+1] - non_zero_acu[offset];
                            float result = 0.0;

                            for(; k < limit; k++){
                                result += receiving_work_values[k] * vector[receiving_work_pos[k]];
                            }

                            sending_buffer[i-offset] = result;
                        }
                    }
                }
                #pragma omp taskwait
            }
        }

        Tcomp += MPI_Wtime() - time_init;

        time_init = MPI_Wtime();

        MPI_Bsend(sending_buffer,chunk_size,MPI_FLOAT,0,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - time_init;

        time_init = MPI_Wtime();
        MPI_Barrier(MPI_COMM_WORLD);
        Tfree += MPI_Wtime() - time_init;

        endTimer(myrank);
    }
    MPI_Finalize();


    return 0;
}