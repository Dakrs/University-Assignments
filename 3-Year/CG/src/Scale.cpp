
#include "../headers/Scale.h"


Scale::Scale(float v[3]) : Transformation() {
    for(int i = 0; i < 3; i++){
        this->scale[i] = v[i];
    }
}

Scale::Scale(Scale * t) : Transformation() {
    for(int i = 0; i < 3; i++){
        scale[i] = t->scale[i];
    }
}

Scale::~Scale(){};

Transformation* Scale::clone() {
    return new Scale(this);
}


void Scale::perform() {
    glScalef(scale[0],scale[1],scale[2]);
}