#include "matrix.h"
#include <ostream>
#include <iostream>
#include <mpi.h>
#include "crs_stattic_master.h"
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

void endTimer(int SIZE,int balanced,int id){
    ending_time = MPI_Wtime();

    double total_time_exectuted = ending_time - starting_time;
    double per_comm = (Tcomm / total_time_exectuted) * 100.0;
    double per_free = (Tfree / total_time_exectuted) * 100.0;
    double per_comp = (Tcomp / total_time_exectuted) * 100.0;

    printf("%d;%5.5lf;%3.2lf;%3.2lf;%3.2lf;\n",id,total_time_exectuted,per_comm,per_free,per_comp);
}

void endTimerMaster(int SIZE,int balanced,int nprocesses){
    ending_time = MPI_Wtime();

    double total_time_exectuted = ending_time - starting_time;
    double per_comm = (Tcomm / total_time_exectuted) * 100.0;
    double per_free = (Tfree / total_time_exectuted) * 100.0;
    double per_comp = (Tcomp / total_time_exectuted) * 100.0;

    printf("MASTER;%5.5lf;%3.2lf;%3.2lf;%3.2lf;\n",total_time_exectuted,per_comm,per_free,per_comp);
}

void endTimerMasterTest(int SIZE,int balanced,int nprocesses,double timer){
    ending_time = timer;

    double total_time_exectuted = ending_time - starting_time;
    double per_comm = (Tcomm / total_time_exectuted) * 100.0;
    double per_free = (Tfree / total_time_exectuted) * 100.0;
    double per_comp = (Tcomp / total_time_exectuted) * 100.0;

    printf("MASTER;%5.5lf;%3.2lf;%3.2lf;%3.2lf;\n",total_time_exectuted,per_comm,per_free,per_comp);
}


int main(int argc, char *argv[])
{
    srand(1);
    int nprocesses; 
    int myrank; 
    double init;

    MPI_Status status;
    MPI_Init(&argc, &argv); 
 
    MPI_Comm_size(MPI_COMM_WORLD, &nprocesses); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    int SIZE = argc > 1 ? atoi(argv[1]) : 4096;
    int chunk_size = argc > 2 ? atoi(argv[2]) : 256;
    int SEED = argc > 3 ? atoi(argv[3]) : 1;
    int bcd = argc > 4 ? atoi(argv[4]) : 1;

    bool balanced = bcd == 1;

    int ROWS = SIZE;
    int COLS = SIZE;


    if (myrank == 0){    
        StatticCRS * server = new StatticCRS(ROWS,COLS,0.2,balanced);
        int chunk_size = ROWS / nprocesses;
        server->setWorkLoad(chunk_size,nprocesses);

        MPI_Barrier(MPI_COMM_WORLD);

        setTimer();

        init = MPI_Wtime();        
        MPI_Bcast(server->vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Bcast(server->non_zero_acu,ROWS+1,MPI_INT,0,MPI_COMM_WORLD);

        for(int i = 1; i < nprocesses; i++){
            server->sendWorkLoad(i);
        }
        Tcomm += MPI_Wtime() - init;

        init = MPI_Wtime();
        server->calZero();
        Tcomp += MPI_Wtime() - init;

        float * result = server->result;

        /**

        init = MPI_Wtime();
        MPI_Gatherv(result,chunk_size,MPI_FLOAT,result,server->chunk_count,server->chunkdiv,MPI_FLOAT,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - init;

        endTimerMaster(SIZE,balanced,nprocesses);*/

        int * chunkdiv = server->chunkdiv;
        int * chunks = server->chunk_count;

        for(int i = 1; i < nprocesses; i++){
            init = MPI_Wtime();
            MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            Tfree += MPI_Wtime() - init;

            init = MPI_Wtime();
            MPI_Recv(result+chunkdiv[status.MPI_SOURCE],chunks[status.MPI_SOURCE],MPI_FLOAT,status.MPI_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            Tcomm += MPI_Wtime() - init;        
        }

        init = MPI_Wtime();
        MPI_Barrier(MPI_COMM_WORLD);
        Tfree += MPI_Wtime() - init;

        init = MPI_Wtime();

        float * result2 = new float[ROWS];
        mult_mat(ROWS,COLS,server->matrix,server->vector,result2);
        for (int c = 0; c < ROWS; c++){
            if (server->result[c] != result2[c]){
                cout << "error at " << c << endl;
                break;
            }
        }
        endTimerMasterTest(SIZE,balanced,nprocesses,init);
    }
    else {
        int * chunkdiv = new int[nprocesses];
        int * chunk_count = new int[nprocesses];
        int chunk_size = ROWS / nprocesses;

        for(int i = 0; i < nprocesses; i++){
            chunkdiv[i] = i*chunk_size;
            chunk_count[i] = (i+1 == nprocesses) ? ROWS / nprocesses + ROWS % chunk_size : chunk_size;
        }

        float * vector = new float[COLS];
        int * non_zero_acu = new int[ROWS+1];

        MPI_Barrier(MPI_COMM_WORLD);

        setTimer();

        init = MPI_Wtime();
        MPI_Bcast(vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        MPI_Bcast(non_zero_acu,ROWS+1,MPI_INT,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - init;

        init = MPI_Wtime();

        int my_chunk_size = chunk_count[myrank];

        float * sending_buffer = new float[my_chunk_size]; //sitio onde se grava

        int offset = chunkdiv[myrank]; //sitio onde começar
        int offsetLimit = chunkdiv[myrank] + my_chunk_size;//sitio onde acaba

        int n_elems_to_receive = non_zero_acu[offsetLimit] - non_zero_acu[offset];

        float * receiving_work_values = new float[n_elems_to_receive]; //buffer para receber
        int * receiving_work_pos = new int[n_elems_to_receive];//buffer para receber
        Tcomp += MPI_Wtime() - init;

        getMessage(MPI_COMM_WORLD);
        init = MPI_Wtime();
        MPI_Recv(receiving_work_values,n_elems_to_receive,MPI_FLOAT,0,1,MPI_COMM_WORLD,&status);
        Tcomm += MPI_Wtime() - init;

        getMessage(MPI_COMM_WORLD);
        init = MPI_Wtime();
        MPI_Recv(receiving_work_pos,n_elems_to_receive,MPI_INT,0,1,MPI_COMM_WORLD,&status);
        Tcomm += MPI_Wtime() - init;

        init = MPI_Wtime();
        for(int i = 0; i < my_chunk_size; i++){
            int k = non_zero_acu[i+offset] - non_zero_acu[offset];
            int limit = non_zero_acu[i+1+offset] - non_zero_acu[offset];
            float result = 0.0;
      
            for(; k < limit; k++)
                result += receiving_work_values[k] * vector[receiving_work_pos[k]];
            sending_buffer[i] = result;
        }
        Tcomp += MPI_Wtime() - init;

        /**
        init = MPI_Wtime();
        MPI_Gatherv(sending_buffer,my_chunk_size,MPI_FLOAT,NULL,NULL,NULL,MPI_FLOAT,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - init;

        endTimer(SIZE,balanced,myrank);*/
        
        init = MPI_Wtime();
        MPI_Send(sending_buffer,my_chunk_size,MPI_FLOAT,0,3,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - init;

        init = MPI_Wtime();
        MPI_Barrier(MPI_COMM_WORLD);
        Tfree += MPI_Wtime() - init;

        endTimer(SIZE,balanced,myrank);
    }
          
    MPI_Finalize();
    return 0;
}