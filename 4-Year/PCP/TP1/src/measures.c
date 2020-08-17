#include "measures.h"

static int comparefloat(const void * a, const void * b){
    float a_value = *(float *) a;
    float b_value = *(float *) b;

    int flag;

    if(a_value == b_value)
        flag = 0;
    else if (a_value < b_value)
        flag = -1;
    else
        flag = 1;
    
    return flag;
}

static int rev_comparefloat(const void * a, const void * b){
    float a_value = *(float *) a;
    float b_value = *(float *) b;

    int flag;

    if(a_value == b_value)
        flag = 0;
    else if (a_value < b_value)
        flag = 1;
    else
        flag = -1;
    
    return flag;
}

static int comparedouble(const void * a, const void * b){
    double a_value = *(double *) a;
    double b_value = *(double *) b;

    int flag;

    if(a_value == b_value)
        flag = 0;
    else if (a_value < b_value)
        flag = -1;
    else
        flag = 1;
    
    return flag;
}

static int rev_comparedouble(const void * a, const void * b){
    double a_value = *(double *) a;
    double b_value = *(double *) b;

    int flag;

    if(a_value == b_value)
        flag = 0;
    else if (a_value < b_value)
        flag = 1;
    else
        flag = -1;
    
    return flag;
}


//reverse == 0 ordena do maior para o menor
float meanKBestFloat(float * arr,int size, int best,int reverse){
    if (reverse == 0)
        qsort(arr, size, sizeof(float), comparefloat);
    else
        qsort(arr, size, sizeof(float), rev_comparefloat);

    float result = 0.0;
    int i;
    for(i = 0; i < size && i < best; i++){
        result += arr[i];
    }

    float mean = result / i;

    return mean;
}


//reverse == 0 ordena do maior para o menor
double meanKBestDouble(double * arr,int size, int best,int reverse){
    if (reverse == 0)
        qsort(arr, size, sizeof(double), comparedouble);
    else
        qsort(arr, size, sizeof(double), rev_comparedouble);

    double result = 0.0;

    int i;
    for(i = 0; i < size && i < best; i++){
        result += arr[i];
    }

    double mean = result / i;

    return mean;
}




static int comparelonglong(const void * a, const void * b){
    long long a_value = *(long long *) a;
    long long b_value = *(long long *) b;

    int flag;

    if(a_value == b_value)
        flag = 0;
    else if (a_value < b_value)
        flag = -1;
    else
        flag = 1;
    
    return flag;
}

static int rev_comparelonglong(const void * a, const void * b){
    long long a_value = *(long long *) a;
    long long b_value = *(long long *) b;

    int flag;

    if(a_value == b_value)
        flag = 0;
    else if (a_value < b_value)
        flag = 1;
    else
        flag = -1;
    
    return flag;
}

float meanKBestLongLong(long long * arr,int size, int best, int reverse){
    if(reverse == 0)
        qsort(arr,size,sizeof(long long),comparelonglong);
    else
        qsort(arr,size,sizeof(long long),rev_comparelonglong);

    long long result = 0;
    
    int i;
    for(i = 0; i < size && i < best; i++){
        result += arr[i];
    }

    float mean = (float) result / i;
    return mean;
}
