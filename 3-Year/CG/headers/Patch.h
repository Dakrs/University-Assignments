
#ifndef _PATCH_H
#define _PATCH_H


#include "Point.h"
#include <vector>

class Patch {
    vector<Point *> * control_points;

public:
    Patch();
    vector<Point *> * getControlPoints();
    void addControlPoint(Point * p);
    Point * pointAt(int i);
    ~Patch();
};


#endif //GENERATOR_PATCH_H
