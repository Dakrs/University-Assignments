
#ifndef GENERATOR_MATERIAL_H
#define GENERATOR_MATERIAL_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif


class Material {

    public:
        Material();
        virtual ~Material();
        virtual void perform();

};


#endif //GENERATOR_MATERIAL_H
