
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "Particle.h"
#include "MD.h"
#include "main.h"
#include <iostream>
#include <fstream>

 void JGFvalidate(MD md){
	if(md.size<2){
	    double refval[]={1731.4306625334357, 7397.392307839352};
	    double dev=abs(md.ek-refval[md.size]);
	    if(dev>1.0e-12)
	    {
	      printf("Validation failed\n");
	      printf("Kinetic Energy = %f %f %d\n",md.ek,dev,md.size);
	    }else printf("Validado\n");
	}else printf("Tamanho nao reconhecido\n");
  }

int main(int argc, char  *argv[]){
    int size = 1;
    int datasizes[3] = {8,13,17};
    MD md;
    Particles particulas;

    if(argc > 1) size = atoi(argv[1]);          // If user set data from Input

    initialiseMD(&md,size,datasizes);           // Inicializar a estrutura MD
    createParticules(&particulas,md.mdsize);    // Criar Particulas
    initialiseParticles(&md,&particulas);       // Inicializar as particulas
    double start = omp_get_wtime();
    runiters(&md,&particulas);                   // Run the algorithm
    double end = omp_get_wtime();
    printf("time: %f seconds.\n", end-start);
    std::cout << md.interactions << std::endl;
    std::ofstream myfile;
    myfile.open("par.txt");
    for(int i = 0; i < md.mdsize; i++){
      myfile << particulas.posX[i] << " - " << particulas.posY[i] << " - " << particulas.posZ[i] << std::endl;
    }
    myfile.close();


    JGFvalidate(md);                            // Validate the values obtained

    return 0;
}
