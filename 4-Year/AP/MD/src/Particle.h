

#ifndef PARTICLE_H
#define	PARTICLE_H



#ifdef	__cplusplus
extern "C" {
#endif
#include "DataTypes.h"

void domove                     (double side, Particles *p, int pos);
inline void calculePow                 (double powDist[]);
inline void storePosition              (double posActPart[],Particles *particulas, int pos);
inline double calculateDistance        (double dist[],double posActPart[],Particles *particulas,int pos,double sideh,MD *md);
void updateForce                (Particles *particulas, double force[], int pos);
void force                      (MD *md,Particles *particulas, int pos);
void third_newton_law           (Particles *particle_oposition,int pos, double leiNewton[]);
void update_static_variables    (double powDist[], MD *md);
void mkekin                     (double hsq2, Particles *p, int pos, double *sum);
void forceAllBlockingParallel(MD *md, Particles *particulas);
void forceAll(MD *md, Particles *particulas);

#ifdef	__cplusplus
}
#endif

#endif	/* PARTICLE_H */
