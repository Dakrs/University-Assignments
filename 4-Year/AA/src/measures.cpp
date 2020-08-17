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

double meanKBestDouble(double * arr,int size, int best,int reverse){
    if (reverse == 0)
        qsort(arr, size, sizeof(double), comparefloat);
    else
        qsort(arr, size, sizeof(double), rev_comparefloat);

    double result = 0.0;

    int i;
    for(i = 0; i < size && i < best; i++){
        result += arr[i];
    }

    double mean = result / i;

    return mean;
}

// verificar se obedece à tolerancia
bool hasConverged(double * arr, int size, int best, float epsilon){
    return (size >= best && ((1+epsilon)*arr[0] >= arr[best-1]));
}