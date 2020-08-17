#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 2000
#define MAXD 42

int tab[N][N] __attribute__ ((aligned(64)));
int tab2[N][N] __attribute__ ((aligned(64)));
int tab3[N][N];

void init_tab() {

    srand(0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j==i) tab[i][j] = 0;
            else tab [i][j] = 1 + rand()%MAXD;
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
          tab2[i][j] = tab[i][j];
          tab3[i][j] = tab[i][j];
        }
}

void copy(){
  for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++){
        tab2[i][j] = tab[i][j];
        tab3[i][j] = tab[i][j];
      }
}

void do_asp() {
	for(int k=0; k<N; k++)
		for(int i=0; i<N; i++)
			if (i != k) {
          for(int j=0; j<N; j++) {
                int tmp = tab[i][k]+tab[k][j];
                if (tmp< tab[i][j]) {
                        tab[i][j] = tmp;
                }
          }

      }
}

/**
#pragma omp declare simd notinbranch
int min(int a, int b){
  return a < b ? a : b;
}

void do_asp_par_vec(){
  for(int k=0; k<N; k++){
    #pragma omp parallel for
    for(int i=0; i<N; i++){
      if(i != k){
        int aux = tab3[i][k];
        #pragma omp simd
        for(int j=0; j<N; j++) {
          tab3[i][j] = min(aux + tab3[k][j],tab3[i][j]);
        }
      }
    }
  }
}*/

void do_asp_par() {

    for(int k=0; k<N; k++){
      #pragma omp parallel for
      for(int i=0; i<N; i+=4){
        if(i != k && (i+1) != k && (i+2) != k && (i+3) != k){
          int aux = tab2[i][k];
          int aux1 = tab2[i+1][k];
          int aux2 = tab2[i+2][k];
          int aux3 = tab2[i+3][k];
          for(int j=0; j<N; j++){
            int tmp = aux + tab2[k][j];
            int tmp2 = aux1 + tab2[k][j];
            int tmp3 = aux2 + tab2[k][j];
            int tmp4 = aux3 + tab2[k][j];
            tab2[i][j] = tmp < tab2[i][j] ? tmp : tab2[i][j];
            tab2[i+1][j] = tmp2 < tab2[i+1][j] ? tmp2 : tab2[i+1][j];
            tab2[i+2][j] = tmp3 < tab2[i+2][j] ? tmp3 : tab2[i+2][j];
            tab2[i+3][j] = tmp4 < tab2[i+3][j] ? tmp4 : tab2[i+3][j];
          }
        }
        else if( i == k ){
          int aux1 = tab2[i+1][k];
          int aux2 = tab2[i+2][k];
          int aux3 = tab2[i+3][k];
          for(int j=0; j<N; j++){
            int tmp2 = aux1 + tab2[k][j];
            int tmp3 = aux2 + tab2[k][j];
            int tmp4 = aux3 + tab2[k][j];
            tab2[i+1][j] = tmp2 < tab2[i+1][j] ? tmp2 : tab2[i+1][j];
            tab2[i+2][j] = tmp3 < tab2[i+2][j] ? tmp3 : tab2[i+2][j];
            tab2[i+3][j] = tmp4 < tab2[i+3][j] ? tmp4 : tab2[i+3][j];
          }
        }
        else if( (i+1) == k ){
          int aux = tab2[i][k];
          int aux2 = tab2[i+2][k];
          int aux3 = tab2[i+3][k];
          for(int j=0; j<N; j++){
            int tmp = aux + tab2[k][j];
            int tmp3 = aux2 + tab2[k][j];
            int tmp4 = aux3 + tab2[k][j];
            tab2[i][j] = tmp < tab2[i][j] ? tmp : tab2[i][j];
            tab2[i+2][j] = tmp3 < tab2[i+2][j] ? tmp3 : tab2[i+2][j];
            tab2[i+3][j] = tmp4 < tab2[i+3][j] ? tmp4 : tab2[i+3][j];
          }
        }
        else if( (i+2) == k ){
          int aux = tab2[i][k];
          int aux1 = tab2[i+1][k];
          int aux3 = tab2[i+3][k];
          for(int j=0; j<N; j++){
            int tmp = aux + tab2[k][j];
            int tmp2 = aux1 + tab2[k][j];
            int tmp4 = aux3 + tab2[k][j];
            tab2[i][j] = tmp < tab2[i][j] ? tmp : tab2[i][j];
            tab2[i+1][j] = tmp2 < tab2[i+1][j] ? tmp2 : tab2[i+1][j];
            tab2[i+3][j] = tmp4 < tab2[i+3][j] ? tmp4 : tab2[i+3][j];
          }
        }
        else if( (i+3) == k ){
          int aux = tab2[i][k];
          int aux1 = tab2[i+1][k];
          int aux2 = tab2[i+2][k];
          for(int j=0; j<N; j++){
            int tmp = aux + tab2[k][j];
            int tmp2 = aux1 + tab2[k][j];
            int tmp3 = aux2 + tab2[k][j];
            tab2[i][j] = tmp < tab2[i][j] ? tmp : tab2[i][j];
            tab2[i+1][j] = tmp2 < tab2[i+1][j] ? tmp2 : tab2[i+1][j];
            tab2[i+2][j] = tmp3 < tab2[i+2][j] ? tmp3 : tab2[i+2][j];
          }
        }
      }
    }
}

void print_tab() {

    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            printf("%d ",tab[i][j]);
        }
        printf("\n");
    }
}
//64314685740
//11601808070
int main() {

    printf("Runnning ASP with %d rows\n",N);

	init_tab();
    //print_tab();

  double total_time = 0.0;
  for(int l = 0; l < 5; l++){
    double start = omp_get_wtime();
    do_asp_par();
    double end = omp_get_wtime() - start;
    printf("Par: %f\n",end);
    total_time += end;
    if (l+1 != 5)
      copy();
  }

  do_asp();
  int ol = 0;
  for (int i=0; i<N && ol == 0; i++) {
      for (int j=0; j<N; j++) {
        if (tab[i][j] != tab2[i][j]){
          printf("Error Par\n");
          ol++;
          break;
        }
      }
  }

	//print_tab();
	return(1);
}
