#include <stdio.h>
#include <string.h>
#include "sort.h"
#include <stdlib.h>
#include <omp.h>

//------------- quick sort
void sort1(int values[], int begin, int end)
{
    int i, j, pivot, aux;
    i = begin;
    j = end-1;
    pivot = values[(begin + end) / 2];
    while(i <= j)
    {
        while(values[i] < pivot && i < end)
        {
            i++;
        }
        while(values[j] > pivot && j > begin)
        {
            j--;
        }
        if(i <= j)
        {
            aux = values[i];
            values[i] = values[j];
            values[j] = aux;
            i++;
            j--;
        }
    }
    if(j > begin)
        sort1(values, begin, j+1);
    if(i < end)
        sort1(values, i, end);
}


// ---------- radix sort (LSD)

#define dig 16

void sort2(int vetor[], int tamanho) {
    int i;
    int *b;
    int maior = vetor[0];
    int exp = 1;

    b = (int *)calloc(tamanho, sizeof(int));

    for (i = 0; i < tamanho; i++) {
        if (vetor[i] > maior)
            maior = vetor[i];
    }

    while (maior/exp > 0) {
        int bucket[dig] = { 0 };
        for (i = 0; i < tamanho; i++)
            bucket[(vetor[i] / exp) % dig]++;
        for (i = 1; i < dig; i++)
            bucket[i] += bucket[i - 1];
        for (i = tamanho - 1; i >= 0; i--)
            b[--bucket[(vetor[i] / exp) % dig]] = vetor[i];
        for (i = 0; i < tamanho; i++)
            vetor[i] = b[i];
        exp *= dig;
    }

    free(b);
}


//--------- heap sort
void sort3(int a[], int n) {
    int i = n / 2, pai, filho, t;
    while(1) {
        if (i > 0) {
            i--;
            t = a[i];
        } else {
            n--;
            if (n == 0) return;
            t = a[n];
            a[n] = a[0];
        }
        pai = i;
        filho = i * 2 + 1;
        while (filho < n) {
            if ((filho + 1 < n)  &&  (a[filho + 1] > a[filho]))
                filho++;
            if (a[filho] > t) {
                a[pai] = a[filho];
                pai = filho;
                filho = pai * 2 + 1;
            } else {
                break;
            }
        }
        a[pai] = t;
    }
}


//--------- merge sort
void aux_sort4(int vetor[], int comeco, int meio, int fim) {
    int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;
    int *vetAux;
    vetAux = (int*)malloc(tam * sizeof(int));

    while(com1 <= meio && com2 <= fim){
        if(vetor[com1] < vetor[com2]) {
            vetAux[comAux] = vetor[com1];
            com1++;
        } else {
            vetAux[comAux] = vetor[com2];
            com2++;
        }
        comAux++;
    }

    while(com1 <= meio){  //Caso ainda haja elementos na primeira metade
        vetAux[comAux] = vetor[com1];
        comAux++;
        com1++;
    }

    while(com2 <= fim) {   //Caso ainda haja elementos na segunda metade
        vetAux[comAux] = vetor[com2];
        comAux++;
        com2++;
    }

    for(comAux = comeco; comAux <= fim; comAux++){    //Move os elementos de volta para o vetor original
        vetor[comAux] = vetAux[comAux-comeco];
    }

    free(vetAux);
}

void sort4(int vetor[], int comeco, int fim){
    if (comeco < fim) {
        int meio = (fim+comeco)/2;

        sort4(vetor, comeco, meio);
        sort4(vetor, meio+1, fim);
        aux_sort4(vetor, comeco, meio, fim);
    }
}

// ---------- radix sort (MSD)

#define digitBits 8
#define digitRange (1<<(digitBits))

int digitAt(int key, int place){
    key = key >> ((place-1)*digitBits);
    return (key%digitRange);
}

void aux_sort5(int array[], int n, int place, int temp[]){
    if (n<=1 || place==0) return;

    // count digits
    int count[digitRange+1] = {0}, i;
    for(i=0;i<n;i++)
      count[ digitAt(array[i], place) ]++;

    for(i=1;i<digitRange+1;i++)
      count[i]+=count[i-1];

    // place keys in a temporary bufffer
    for (i=0; i<n; i++)
      temp[-1+(count[digitAt(array[i], place)]--)]=array[i];

    // copy to original array
    for (i=0; i<n; i++)
      array[i]=temp[i];

    // recurive sort


    for (i=0;i<digitRange;i++){
      aux_sort5(array+count[i], count[i+1]-count[i], place-1, temp+count[i]);
    }
    //printf("%d\n",digitRange);
}


void sort5(int array[], int tam) {
    int *vetAux,i;
    vetAux = (int*)malloc(tam * sizeof(int));
    // find max digit
    int max=array[0];
    for (i=1;i<tam;i++){
        if(array[i]>max) max=array[i];
    }
    int maxd=1;
    printf("%d\n",max);
    while(max/digitRange>0) {
        max=max/digitRange;
        maxd++;
    }
    printf("%d\n",maxd);
    aux_sort5(array,tam,maxd,vetAux);
}

#define digitBitsSplit 5
#define digitRangeSplit (1<<(digitBitsSplit))

int digitAtForSplit(int key){
    key = key >> 22;//21-6
    return (key%digitRangeSplit);
}

void aux_sort6(int array[], int n, int place, int temp[]){
    if (n<=1 || place==0) return;

    // count digits
    int count[digitRange+1] = {0}, i;
    for(i=0;i<n;i++)
      count[ digitAt(array[i], place) ]++;

    for(i=1;i<digitRange+1;i++)
      count[i]+=count[i-1];

    // place keys in a temporary bufffer
    for (i=0; i<n; i++)
      temp[-1+(count[digitAt(array[i], place)]--)]=array[i];

    // copy to original array
    for (i=0; i<n; i++)
      array[i]=temp[i];

    // recurive sort
    for (i=0;i<digitRange;i++){
      aux_sort6(array+count[i], count[i+1]-count[i], place-1, temp+count[i]);
    }
}

void split_work(int array[], int n, int place, int temp[]){
    if (n<=1 || place==0) return;

    // count digits
    int count[digitRangeSplit+1] = {0}, i;
    #pragma omp parallel for reduction(+:count[:digitRangeSplit+1])
    for(i=0;i<n;i++)
      count[ digitAtForSplit(array[i]) ]++;

    for(i=1;i<digitRangeSplit+1;i++)
      count[i]+=count[i-1];

    for (i=0; i<n; i++){
      temp[-1+(count[digitAtForSplit(array[i])]--)]=array[i];
    }

    #pragma omp parallel for
    for (i=0; i<n; i++)
      array[i]= temp[i];

    #pragma omp parallel for
    for (i=0;i<digitRangeSplit;i++){
      aux_sort6(array+count[i], count[i+1]-count[i], place-1, temp+count[i]);
    }
}

/**
Radix MSD Parallel
*/

void sort6(int array[], int tam) {
    int *vetAux,i;
    vetAux = (int*)malloc(tam * sizeof(int));
    // find max digit
    int max=array[0];
    for (i=1;i<tam;i++){
        if(array[i]>max) max=array[i];
    }
    int maxd=1;
    while(max/digitRange>0) {
        max=max/digitRange;
        maxd++;
    }
    split_work(array,tam,maxd,vetAux);
}
