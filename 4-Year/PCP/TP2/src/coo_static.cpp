#include "coo_static.h"

StaticCOO::StaticCOO(int n_rows,int n_cols,float p_non_zeros,bool balanced){
    int n_elem = n_rows * n_cols;
    int n_zeros = ceil(n_cols*p_non_zeros) * n_rows;

    matrix = new float[n_elem];

    non_zero_values = new float[n_zeros];
    non_zero_cols = new int[n_zeros];
    non_zero_rows = new int[n_zeros];

    vector = new float[n_cols];
    result = new float[n_rows];

    if (balanced)
        rand_sparse_vec_balanced(n_rows,n_cols,matrix,p_non_zeros);
    else{
        rand_sparse_vec(n_rows,n_cols,matrix,p_non_zeros);
    }
    rand_vector(n_cols,vector);

    for(int i = 0; i < n_rows; i++)
        result[i] = 0.0;

    ROWS = n_rows;
    COLS = n_cols;

    transform();

    zeros = n_zeros;
}

void StaticCOO::transform(){
    int n = 0;
    float val;

    for(int i = 0; i < ROWS; i++){
      for (int j = 0; j < COLS; j++)
      {
          val = matrix[i*COLS + j];
          if (val != 0.0){
            non_zero_rows[n] = i;
            non_zero_cols[n] = j;
            non_zero_values[n] = val;
            n++;
          }
      }
    }
}

void StaticCOO::print(){
    cout << "Matrix entrada" << endl;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            cout << matrix[i*COLS + j] << " - ";
        }
            cout << endl;
    }

    cout << "Valores nao zero" << endl;
    for (int i = 0; i < zeros; i++)
    {
        cout << non_zero_values[i] << " - ";
    }
    cout << endl << "Linhas nao zero" << endl;
    for (int i = 0; i < zeros; i++)
    {
        cout << non_zero_rows[i] << " - ";
    }
    cout << endl << "Colunas nao zero" << endl;
    for (int i = 0; i < zeros; i++)
    {
        cout << non_zero_cols[i] << " - ";
    }
    cout << endl;
}

void StaticCOO::setWorkLoad(int chunk_size, int np){
    this->chunk_size = chunk_size;
    this->chunkdiv = new int[np];
    this->chunk_count = new int[np];
    for(int i = 0; i < np; i++){
        chunkdiv[i] = i*chunk_size;
        chunk_count[i] = (i+1 == np) ? zeros / np + zeros % chunk_size : chunk_size;
    }
}

void StaticCOO::sendWorkLoad(int destination){
    int offset = destination * chunk_size;
    work_sent += chunk_size;

    MPI_Send(non_zero_values + offset,chunk_size,MPI_FLOAT,destination,1,MPI_COMM_WORLD);
    MPI_Send(non_zero_rows + offset,chunk_size,MPI_INT,destination,1,MPI_COMM_WORLD);
    MPI_Send(non_zero_cols + offset,chunk_size,MPI_INT,destination,1,MPI_COMM_WORLD);
}

void StaticCOO::computeZero(){
    for(int i = 0; i < chunk_size; i++){
        result[non_zero_rows[i]] += non_zero_values[i] * vector[non_zero_cols[i]];
    }
}

void StaticCOO::updateResult(float * buffer){
    for (int i = 0; i < ROWS; i++)
    {
        result[i] += buffer[i];
    }
}

