
#include "../headers/Light.h"



Light::Light(int type, float *pos) {
    this->type = type;
    this->position[0] = pos[0];
    this->position[1] = pos[1];
    this->position[2] = pos[2];
    this->position[3] = type == 0 ? 1.0 : 0.0;
}

Light::Light(int type, float *pos, float * dir ,float *aux) {
    this->type = type;
    this->position[0] = pos[0];
    this->position[1] = pos[1];
    this->position[2] = pos[2];
    this->position[3] = 1.0;

    this->direction[0] = dir[0];
    this->direction[1] = dir[1];
    this->direction[2] = dir[2];

    this->extras[0] = aux[0];
    this->extras[1] = aux[1];
}

Light::~Light() {}

void Light::draw() {
    GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

    if (type == 2){
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, extras[0]);
        glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, extras[1]);
    }
}
