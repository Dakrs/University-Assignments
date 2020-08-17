

#ifndef _DIFFUSE_H
#define _DIFFUSE_H


#include "Material.h"

class Diffuse : public Material {
    float color[4];

    public: Diffuse(float cor [3]);
            ~Diffuse();
            void perform();
};


#endif //_DIFFUSE_H
