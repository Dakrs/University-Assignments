#include "matrix.h"
#include <ostream>
#include <iostream>
#include <mpi.h>
#include "coo_static.h"
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

    printf("MASTER;%5.5lf;%3.2lf;%3.2lf;%3.2lf;",total_time_exectuted,per_comm,per_free,per_comp);
}


int main(int argc, char *argv[])
{
    srand(1);
    int nprocesses; 
    int myrank; 

    MPI_Status status;
    MPI_Init(&argc, &argv); 
 
    MPI_Comm_size(MPI_COMM_WORLD, &nprocesses); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    double init;
    
    int SIZE = argc > 1 ? atoi(argv[1]) : 4096;
    int chunk_size = argc > 2 ? atoi(argv[2]) : 256;
    int SEED = argc > 3 ? atoi(argv[3]) : 1;
    int bcd = argc > 4 ? atoi(argv[4]) : 1;

    bool balanced = bcd == 1;

    int ROWS = SIZE;
    int COLS = SIZE;
    int ZEROS = ceil(COLS*0.2) * ROWS;

    StaticCOO * server;
    
    float * sending_buffer = new float[ROWS];
    float * vector = new float[COLS];

    float * values;
    int * rows;
    int * cols;

    for(int k = 0; k < ROWS; k++){
        sending_buffer[k] = 0.0;
    }  

    if (myrank == 0){
        server = new StaticCOO(ROWS,COLS,0.2,1);
        chunk_size = ZEROS / nprocesses;
        server->setWorkLoad(chunk_size,nprocesses);
        float * receiver = new float[ROWS];

        MPI_Barrier(MPI_COMM_WORLD);
        setTimer();
        
        init = MPI_Wtime(); 
        MPI_Bcast(server->vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - init;

        init = MPI_Wtime(); 
        for(int i = 1; i < nprocesses; i++){
            server->sendWorkLoad(i);
        }
        Tcomm += MPI_Wtime() - init;

        init = MPI_Wtime();
        server->computeZero();
        Tcomp += MPI_Wtime() - init;

        for(int i = 1; i < nprocesses; i++){
            getMessage(MPI_COMM_WORLD);
            init = MPI_Wtime();
            MPI_Recv(receiver,ROWS,MPI_FLOAT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
            Tcomm += MPI_Wtime() - init;

            init = MPI_Wtime();
            server->updateResult(receiver);
            Tcomp += MPI_Wtime() - init;
        }

        endTimerMaster(SIZE,balanced,nprocesses);
    }

    if(myrank != 0){

        int * chunkdiv = new int[nprocesses];
        int * chunk_count = new int[nprocesses];
        int chunk_size = ZEROS / nprocesses;

        for(int i = 0; i < nprocesses; i++){
            chunkdiv[i] = i*chunk_size;
            chunk_count[i] = (i+1 == nprocesses) ? ZEROS / nprocesses + ZEROS % chunk_size : chunk_size;
        }

        int my_chunk_size = chunk_count[myrank];

        float * nzv = new float[my_chunk_size];
        int * nzc = new int[my_chunk_size];
        int * nzr = new int[my_chunk_size];

        MPI_Barrier(MPI_COMM_WORLD);
        setTimer();

        init = MPI_Wtime();
        MPI_Bcast(vector,COLS,MPI_FLOAT,0,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - init;

        getMessage(MPI_COMM_WORLD);
        init = MPI_Wtime();
        MPI_Recv(nzv,my_chunk_size,MPI_FLOAT,0,1,MPI_COMM_WORLD,&status);
        Tcomm += MPI_Wtime() - init;

        getMessage(MPI_COMM_WORLD);
        init = MPI_Wtime();
        MPI_Recv(nzr,my_chunk_size,MPI_INT,0,1,MPI_COMM_WORLD,&status);
        Tcomm += MPI_Wtime() - init;

        getMessage(MPI_COMM_WORLD);
        init = MPI_Wtime();
        MPI_Recv(nzc,my_chunk_size,MPI_INT,0,1,MPI_COMM_WORLD,&status);
        Tcomm += MPI_Wtime() - init;

        init = MPI_Wtime();
        for(int k = 0; k < my_chunk_size; k++){
            sending_buffer[nzr[k]] += vector[nzc[k]] * nzv[k];
        }
        Tcomp += MPI_Wtime() - init;

        init = MPI_Wtime();
        MPI_Send(sending_buffer,ROWS,MPI_FLOAT,0,1,MPI_COMM_WORLD);
        Tcomm += MPI_Wtime() - init;

        endTimer(SIZE,balanced,myrank);
    }
    
    if(myrank == 0){
        
        float * result2 = new float[ROWS];
        mult_mat(ROWS,COLS,server->matrix,server->vector,result2);

        for (int c = 0; c < ROWS; c++)
        {
            if(result2[c] != server->result[c])
            printf("devia ter dado %f deu %f na posicao: %d\n",result2[c], server->result[c],c);   
            break;
        }
    }
    
    MPI_Finalize();
    return 0;
    
}
