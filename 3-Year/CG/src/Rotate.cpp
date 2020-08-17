#include "../headers/Rotate.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <cstdio>

Rotate::Rotate(float time, float angle, int v[3]) : Transformation() {
    for(int i = 0; i < 3; i++){
        this->axis[i] = v[i];
    }
    this->time = time;
    this->angle = angle;
}

Rotate::Rotate(Rotate * t) : Transformation() {
    for(int i = 0; i < 3; i++){
        axis[i] = t->axis[i];
    }
    time = t->time;
    angle = t->angle;
}

Rotate::~Rotate(){};

Transformation* Rotate::clone() {
    return new Rotate(this);
}


void Rotate::perform() {
    if (this->time != 0) {
        float i, angle = 360 * modff((glutGet(GLUT_ELAPSED_TIME) / (double) (time * 1000)), &i); // modf not working on windowns
        glRotatef(angle, axis[0], axis[1], axis[2]);
    }
    else if (this->angle != 0){
        glRotatef(angle,axis[0],axis[1],axis[2]);
    }
}