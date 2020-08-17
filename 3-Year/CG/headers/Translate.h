

#ifndef TRANSLATE_H
#define TRANSLATE_H


#include "Transformation.h"
#include "Point.h"
#include <vector>
#include <iostream>

class Translate : public Transformation {
    vector<Point *> * control_points;
    float duration;

    public: Translate(float time);
            void addPoint(Point * p);
            Translate(Translate * t);
            ~Translate();
            Transformation* clone();
            void perform();

    private:
            void renderCatmullRomCurve();
            void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);
            void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv);

};

void buildRotMatrix(float *x, float *y, float *z, float *m);
void cross(float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
void multMatrixVector(float *m, float *v, float *res);


#endif //TRANSLATE_H
