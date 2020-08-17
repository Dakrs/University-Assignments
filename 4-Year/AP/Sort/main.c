#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.h"

static int ini_vector (int **v, int n);
static int alloc_vector (int **v, int n);
static int copy_vector (int *v1, int *v2, int n);
static int free_vector (int **v);


int main (int argc, char *argv[]) {

    int *a, *master;

    // read parameters
    if (argc != 4) {
        fprintf (stdout, "sort algorithm runs size \n");
        return 1;
    }
    int alg = atoi(argv[1]);
    int runs = atoi(argv[2]);
    int arraySize = atoi(argv[3]);


    // ini Master vector
    //  fprintf (stdout, "Initializing Vector with %d elements ... ", arraySize);
    if (!ini_vector (&master, arraySize)) return 0;
    //  fprintf (stdout, "done!\n");

    // alloc sorting vector a
    //  fprintf (stdout, "Alloc sort vector ...");
    if (!alloc_vector (&a, arraySize)) return 0;
    //  fprintf (stdout, "done!\n");

    for (int run=0 ; run < runs ; run++) {

        // copy n elements of master to a
        copy_vector (master, a, arraySize);

        //    fprintf (stderr, "run=%d ... ;", run);
        double start = omp_get_wtime();
        switch (alg)
        {
            case 1:
                sort1(a, 0, arraySize);
                break;
            case 2:
                sort2(a, arraySize);
                break;
            case 3:
                sort3(a, arraySize);
                break;
            case 4:
                sort4(a, 0, arraySize);
                break;
            case 5:
                sort5(a,arraySize);
                break;
            case 6:
                sort6(a,arraySize);
                break;
            default:
                fprintf (stderr, "Insert correct option...");
        }
        double end = omp_get_wtime();
        printf("Run %d: %f seconds.\n",run, end-start);

    } // end runs

    int ok=1;
    for(int i=0; i<arraySize-1 && ok; i++)
        if (a[i]>a[i+1]) { ok=0; printf("sorting error: %d > %d\n",a[i],a[i+1]); break; }
    if (ok) printf("sorting ok\n");

    free_vector (&a);
    free_vector (&master);

    return 1;
}

int alloc_vector (int **v, int n) {

    *v = (int *) malloc (n*sizeof(int));
    if (!(*v)) {
        fprintf (stderr, "Could not allocate memory for vector!");
        return 0;
    }
    return 1;
}

int ini_vector (int **v, int n) {
    int i;
    int *ptr;

    if (!alloc_vector (v, n)) return 0;
    for (i=0 , ptr = (*v) ; i<n ; i++ , ptr++) {
      *ptr = rand() % 100000000;
      //*ptr = rand() % 134217728;
      //*ptr = rand() % 100;
    }
    return 1;
}

int free_vector (int **v) {
    free (*v);
    *v = NULL;
    return 1;
}

int copy_vector (int *v1, int *v2, int n) {
    int i;

    for (i=0 ; i<n ; i++ , v1++, v2++) {
        *v2 = *v1;
    }
    return 1;
}
