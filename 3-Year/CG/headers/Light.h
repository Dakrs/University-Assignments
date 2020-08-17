

#ifndef _LIGHT_H
#define _LIGHT_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


class Light {
    int type;
    float position[4];
    float direction[3];
    float extras[2];

public: Light(int type, float pos[3]); //0 point //1 DIRECTIONAL
        Light(int type, float pos[3], float dir[3] ,float aux[2]);  //2 SPOT
        ~Light();
        void draw();
};


#endif //_LIGHT_H
