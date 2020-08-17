

#include "../../headers/Specular.h"


Specular::Specular(float *cor,float shinny) {
    this->color[0] = cor[0];
    this->color[1] = cor[1];
    this->color[2] = cor[2];
    this->color[3] = 1.0f;

    this->shininess = shinny;

}

Specular::~Specular(){}

void Specular::perform() {
    glMaterialfv(GL_FRONT,GL_SPECULAR,color);
    glMaterialf(GL_FRONT,GL_SHININESS,shininess);
}