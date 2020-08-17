
#ifndef _SPECULAR_H
#define _SPECULAR_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include "Material.h"

class Specular : public Material {
    float color[4];
    float shininess;

    public: Specular(float cor[3],float shinny);
            ~Specular();
            void perform();
};


#endif //_SPECULAR_H
