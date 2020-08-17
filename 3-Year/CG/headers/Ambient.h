

#ifndef _AMBIENT_H
#define _AMBIENT_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include "Material.h"

class Ambient : public Material {
    float color[4];


    public: Ambient(float cor[3]);
            ~Ambient();
            void perform();
};


#endif //_AMBIENT_H
