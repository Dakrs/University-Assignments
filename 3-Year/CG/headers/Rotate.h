#ifndef ROTATE_H
#define ROTATE_H


#include "Transformation.h"

class Rotate : public Transformation {
    int axis[3];
    float time;
    float angle;

public: Rotate(float time, float angle, int v[3]);
    Rotate(Rotate * t);
    ~Rotate();
    Transformation* clone();
    void perform();

};

#endif
