

#ifndef SCALE_H
#define SCALE_H


#include "Transformation.h"

class Scale : public Transformation {
    float scale[3];

public: Scale(float v[3]);
    Scale(Scale * t);
    ~Scale();
    Transformation* clone();
    void perform();

};

#endif //SCALE_H
