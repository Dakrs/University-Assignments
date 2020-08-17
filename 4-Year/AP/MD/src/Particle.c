#include "DataTypes.h"
#include "Particle.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>




// Fazer a movimentação das particulas
 void domove(double side, Particles *p, int pos){
        p->posX[pos] += p->vX[pos] + p->fX[pos];
        p->posY[pos] += p->vY[pos] + p->fY[pos];
        p->posZ[pos] += p->vZ[pos] + p->fZ[pos];

     if (p->posX[pos] < 0)    p->posX[pos] += side;
        if (p->posX[pos] > side) p->posX[pos] -= side;
        if (p->posY[pos] < 0)    p->posY[pos] += side;
        if (p->posY[pos] > side) p->posY[pos] -= side;
        if (p->posZ[pos] < 0)    p->posZ[pos] += side;
        if (p->posZ[pos]> side)  p->posZ[pos] -= side;

        p->vX[pos] += p->fX[pos];
        p->vY[pos] += p->fY[pos];
        p->vZ[pos] += p->fZ[pos];

        p->fX[pos] = p->fY[pos] = p->fZ[pos] = 0.0;
}

inline void calculePow(double powDist[]){
        powDist[1] = 1.0/powDist[0];
        powDist[2] = powDist[1] * powDist[1];
        powDist[3] = powDist[2] * powDist[1];
        powDist[4] = powDist[2] * powDist[2];
        powDist[6] = powDist[2] * powDist[4];
        powDist[7] = powDist[6] * powDist[1];
        powDist[8] = powDist[7] - 0.5 * powDist[4];
 }

inline void storePosition(double posActPart[],Particles *particulas, int pos){
     posActPart[0] = particulas->posX[pos];       // Guardar a posição x da particula
     posActPart[1] = particulas->posY[pos];       // Guardar a posição y da particula
     posActPart[2] = particulas->posZ[pos];       // Guardar a posição z da particula
 }

inline double calculateDistance(double dist[],double posActPart[],Particles *particulas,int pos,double sideh,MD *md){
     dist[0] = posActPart[0] - particulas->posX[pos];       // Determinar distancia em x entre as particulas
     dist[1] = posActPart[1] - particulas->posY[pos];       // Determinar distancia em y entre as particulas
     dist[2] = posActPart[2] - particulas->posZ[pos];       // Determinar distancia em z entre as particulas

     if(dist[0] < (-sideh)) { dist[0] += md->side; }         // Verificar que não sai dos lados
     if(dist[0] > (sideh))  { dist[0] -= md->side; }
     if(dist[1] < (-sideh)) { dist[1] += md->side; }
     if(dist[1] > (sideh))  { dist[1] -= md->side; }
     if(dist[2] < (-sideh)) { dist[2] += md->side; }
     if(dist[2] > (sideh))  { dist[2] -= md->side; }

     return dist[0]*dist[0] + dist[1]*dist[1] + dist[2]*dist[2]; // Modulo da distancia entre as particulas

}

void updateForce(Particles *particulas, double force[], int pos){
     particulas->fX[pos] += force[0];
     particulas->fY[pos] += force[1];
     particulas->fZ[pos] += force[2];
}


void forceAllBlockingParallel(MD *md, Particles *particulas){
  md->epot = md->vir = 0.0;
  double sideh = 0.5 * md->side, rcoffs = md->rcoff * md->rcoff;
  double posActPart[3];   // Guarda as posições da particula passada por referencia
  double dist[3];         // Guarda as distancia entre as duas particulas // Guarda as forças
  double powDist[9];      // Guarda as potencias das distancias
  double leiNewton[3];

  int blockY = 256;
  int n_blocksY = ceil((float) md->mdsize / (float) blockY);

  omp_lock_t lock[n_blocksY];

  for (int i=0; i < n_blocksY; i++)
    omp_init_lock(&(lock[i]));

  #pragma omp parallel
  {
    double forceX[md->mdsize] = {0};
    double forceY[md->mdsize] = {0};
    double forceZ[md->mdsize] = {0};
    int block_index[n_blocksY] = {0};
    double epot = 0.0;
    double vir = 0.0;
    int iter = 0;

    #pragma omp for schedule(static, 1) private(posActPart,dist,leiNewton,powDist) collapse(2)
    for(int id = 0; id < n_blocksY; id++){
      for(int jd = 0; jd < n_blocksY; jd++){
        if (jd < id)
          continue;
        int i = blockY * id;
        int j = blockY * jd;
        int maxi = (i + blockY) >= md->mdsize ? md->mdsize : (i + blockY);

        for(int ii = i; ii < maxi; ii++){
          int maxj = (j + blockY) >= md->mdsize ? md->mdsize : (j + blockY);
          int jj = j <= ii ? ii + 1 : j;
          //storePosition(posActPart,particulas,ii);
          posActPart[0] = particulas->posX[ii];       // Guardar a posição x da particula
          posActPart[1] = particulas->posY[ii];       // Guardar a posição y da particula
          posActPart[2] = particulas->posZ[ii];

          for(; jj < maxj; jj++){
            //powDist[0] = calculateDistance(dist,posActPart,particulas,jj,sideh,md);
            dist[0] = posActPart[0] - particulas->posX[jj];       // Determinar distancia em x entre as particulas
            dist[1] = posActPart[1] - particulas->posY[jj];       // Determinar distancia em y entre as particulas
            dist[2] = posActPart[2] - particulas->posZ[jj];       // Determinar distancia em z entre as particulas

            if(dist[0] < (-sideh)) { dist[0] += md->side; }         // Verificar que não sai dos lados
            if(dist[0] > (sideh))  { dist[0] -= md->side; }
            if(dist[1] < (-sideh)) { dist[1] += md->side; }
            if(dist[1] > (sideh))  { dist[1] -= md->side; }
            if(dist[2] < (-sideh)) { dist[2] += md->side; }
            if(dist[2] > (sideh))  { dist[2] -= md->side; }

            powDist[0] = dist[0]*dist[0] + dist[1]*dist[1] + dist[2]*dist[2]; // Modulo da distancia entre as particulas

            if(powDist[0] <= rcoffs){
              //calculePow(powDist);
              powDist[1] = 1.0/powDist[0];
              powDist[2] = powDist[1] * powDist[1];
              powDist[3] = powDist[2] * powDist[1];
              powDist[4] = powDist[2] * powDist[2];
              powDist[6] = powDist[2] * powDist[4];
              powDist[7] = powDist[6] * powDist[1];
              powDist[8] = powDist[7] - 0.5 * powDist[4];

              leiNewton[0] = dist[0] * powDist[8];
              forceX[ii] += leiNewton[0];
              leiNewton[1] = dist[1] * powDist[8];
              forceY[ii] += leiNewton[1];
              leiNewton[2] = dist[2] * powDist[8];
              forceZ[ii] += leiNewton[2];

              forceX[jj] -= leiNewton[0];
              forceY[jj] -= leiNewton[1];
              forceZ[jj] -= leiNewton[2];

              epot += powDist[6] - powDist[3];// (rrd6 - rrd3);
              vir  += powDist[0] * powDist[8];// rd * r148;
              iter++;
            }
          }
        }
        block_index[id] = 1;
        block_index[jd] = 1;
      }
    }

    for(int i = 0; i < n_blocksY; i++){
      if (block_index[i] == 1){
        omp_set_lock(&(lock[i]));
        int max = (i+1) == n_blocksY ? md->mdsize : (i+1)*blockY;
        for(int k = i * blockY; k < max; k++){
          particulas->fX[k] += forceX[k];
          particulas->fY[k] += forceY[k];
          particulas->fZ[k] += forceZ[k];
        }
        omp_unset_lock(&(lock[i]));
      }
    }

    #pragma omp atomic update
    md->epot += epot;
    #pragma omp atomic update
    md->vir -= vir;
    #pragma omp atomic update
    md->interactions += iter;
  }


  for (int i=0; i < n_blocksY; i++)
    omp_destroy_lock(&(lock[i]));
}

void forceAll(MD *md, Particles *particulas){
  md->epot = md->vir = 0.0;
  double sideh = 0.5 * md->side, rcoffs = md->rcoff * md->rcoff;
  double posActPart[3];   // Guarda as posições da particula passada por referencia
  double dist[3];         // Guarda as distancia entre as duas particulas
  double force[3]={0,0,0}; // Guarda as forças
  double powDist[9];      // Guarda as potencias das distancias
  double leiNewton[3];


  #pragma omp parallel
  {
    double epot = 0.0;
    double vir = 0.0;
    int iter = 0;

    #pragma omp for schedule(static, 64) private(posActPart,force,dist,leiNewton,powDist)
    for(int i = 0; i < md->mdsize; i++){
      force[0] = 0;
      force[1] = 0;
      force[2] = 0;
      storePosition(posActPart,particulas,i);
      for(int j = i + 1; j < md->mdsize; j++){
        powDist[0] = calculateDistance(dist,posActPart,particulas,j,sideh,md);

        if(powDist[0] <= rcoffs){
          calculePow(powDist);
          leiNewton[0] = dist[0] * powDist[8];
          force[0] += leiNewton[0];
          leiNewton[1] = dist[1] * powDist[8];
          force[1] += leiNewton[1];
          leiNewton[2] = dist[2] * powDist[8];
          force[2] += leiNewton[2];

          #pragma omp atomic update
          particulas->fX[j] -= leiNewton[0];
          #pragma omp atomic update
          particulas->fY[j] -= leiNewton[1];
          #pragma omp atomic update
          particulas->fZ[j] -= leiNewton[2];

          epot += powDist[6] - powDist[3];// (rrd6 - rrd3);
          vir  += powDist[0] * powDist[8];// rd * r148;
          iter++;
        }
      }

      #pragma omp atomic update
      particulas->fX[i] += force[0];
      #pragma omp atomic update
      particulas->fY[i] += force[1];
      #pragma omp atomic update
      particulas->fZ[i] += force[2];
    }

    #pragma omp critical
    {
      md->epot += epot;
      md->vir -= vir;
      md->interactions += iter;
    }
  }
}

void force(MD *md,Particles *particulas, int pos){
     double sideh = 0.5 * md->side, rcoffs = md->rcoff * md->rcoff;
     double posActPart[3];   // Guarda as posições da particula passada por referencia
     double dist[3];         // Guarda as distancia entre as duas particulas
     double force[3]={0,0,0}; // Guarda as forças
     double powDist[9];      // Guarda as potencias das distancias
     double leiNewton[3];
     int i;

     storePosition(posActPart,particulas,pos);

     for (i = pos + 1; i < md ->mdsize; i++) {

        powDist[0] = calculateDistance(dist,posActPart,particulas,i,sideh,md);
        if(powDist[0] <= rcoffs)
        {
           calculePow(powDist);
           leiNewton[0] = dist[0] * powDist[8];
           force[0] += leiNewton[0];
           leiNewton[1] = dist[1] * powDist[8];
           force[1] += leiNewton[1];
           leiNewton[2] = dist[2] * powDist[8];
           force[2] += leiNewton[2];
           third_newton_law(particulas,i, leiNewton);
           update_static_variables(powDist, md);
        }

       }
     updateForce(particulas,force,pos);
  }


void third_newton_law(Particles *particle_oposition,int pos, double leiNewton[]){
    particle_oposition->fX[pos] -= leiNewton[0];
    particle_oposition->fY[pos] -= leiNewton[1];
    particle_oposition->fZ[pos] -= leiNewton[2];
}

void update_static_variables(double powDist[], MD *md){
     md->epot += powDist[6] - powDist[3];// (rrd6 - rrd3);
     md->vir  -= powDist[0] * powDist[8];// rd * r148;
     md->interactions++;
}


void mkekin(double hsq2, Particles *p, int pos, double *sum) {

     p->fX[pos] *= hsq2;
     p->fY[pos] *= hsq2;
     p->fZ[pos] *= hsq2;

     p->vX[pos] += p->fX[pos];
     p->vY[pos] += p->fY[pos];
     p->vZ[pos] += p->fZ[pos];

     *sum += p->vX[pos] * p->vX[pos] +
             p->vY[pos] * p->vY[pos] +
             p->vZ[pos] * p->vZ[pos];
   }
