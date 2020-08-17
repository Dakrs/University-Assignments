

#ifndef _EMISSIVE_H
#define _EMISSIVE_H


#include "Material.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif

class Emissive : public Material {
    float color[4];

    public: Emissive(float cor[3]);
            ~Emissive();
            void perform();
};


#endif //_EMISSIVE_H
