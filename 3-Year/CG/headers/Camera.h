

#ifndef _CAMERA_H
#define _CAMERA_H
#define _USE_MATH_DEFINES
#include <cmath>
#include "Point.h"
#include "Figure.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Camera {
    Point * position;
    Point * lookup;
    Point * up;
    float alpha;
    float beta;
    float radius;
    float speed;
    int third;
    Figure * fig;

    public:
        Camera();
        Point * getPosition();
        Point * getPositionOld();
        Point * getLookup();
        Point * getUp();
        void thirdperson();
        void updateCamera(float a,float b);
        ~Camera();
        void move(unsigned char c, int xx, int yy);
        void draw();
        void setFig(Figure * f);

    private:
        void updateLookup();
        void updateUp();


};


#endif //GENERATOR_CAMERA_H
