
#include "../../headers/Diffuse.h"


Diffuse::Diffuse(float *cor) {
    this->color[0] = cor[0];
    this->color[1] = cor[1];
    this->color[2] = cor[2];
    this->color[3] = 1.0f;
}

Diffuse::~Diffuse() {}

void Diffuse::perform() {
    glMaterialfv(GL_FRONT,GL_DIFFUSE,color);
}
