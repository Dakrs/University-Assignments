

#include "../../headers/Emissive.h"


Emissive::Emissive(float *cor) {
    color[0] = cor[0];
    color[1] = cor[1];
    color[2] = cor[2];
    color[3] = 1.0f;
}

Emissive::~Emissive() {}

void Emissive::perform() {
    glMaterialfv(GL_FRONT,GL_EMISSION,color);
}