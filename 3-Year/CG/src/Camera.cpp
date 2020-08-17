

#include <iostream>
#include "../headers/Camera.h"


static void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

Camera::Camera() {
    position = new Point(40,20,0);
    lookup = new Point(0,0,0);
    up = new Point(0,1,0);
    alpha = 0.0;
    beta = M_PI / 6;
    radius = 40;
    speed = 0.01;
    updateUp();
    updateLookup();
    fig = NULL;
}

Point * Camera::getPosition() {
    if (third == 1){
        float res[3];
        this->position->get(res);

        float alphaN = M_PI + alpha;
        float betaN = - beta + M_PI / 3.0;

        float x = -2.0 * cos(betaN) * cos(alphaN) + position->getX();
        float y = 2.0 * sin(betaN) + position->getY();
        float z = -2.0 * cos(betaN) * sin(alphaN) + position->getZ();

        return new Point(x,y,z);
    }
    return position;
}

Point * Camera::getPositionOld(){
    return position;
}

Point * Camera::getUp() {
    return up;
}

Point * Camera::getLookup() {
    return lookup;
}

void Camera::updateCamera(float a,float b){
    alpha += a;
    beta += b;

    if (beta >= (M_PI / 2.0))
        beta = (M_PI / 2.0);

    if (beta <= -(M_PI / 2.0))
        beta = -(M_PI / 2.0);

    updateLookup();
    updateUp();
}

Camera::~Camera() {}

void Camera::updateUp(){

    up->setX(1.0 * cos(beta + (M_PI / 2)) * cos(alpha));
    up->setY(1.0 * sin(beta + (M_PI / 2)));
    up->setZ(1.0 * cos(beta + (M_PI / 2)) * sin(alpha));
}

void Camera::updateLookup() {
    lookup->setX(-radius * cos(beta) * cos(alpha) + position->getX());
    lookup->setY(-radius * sin(beta) + position->getY());
    lookup->setZ(-radius * cos(beta) * sin(alpha) + position->getZ());
}

void Camera::move(unsigned char c, int xx, int yy){
    float x,y,z;

    float res[3];
    float d[3],upA[3];

    x = position->getX();
    y = position->getY();
    z = position->getZ();
    d[0] = speed * (lookup->getX() - x);
    d[1] = speed * (lookup->getY() - y);
    d[2] = speed * (lookup->getZ() - z);
    upA[0] = up->getX();
    upA[1] = up->getY();
    upA[2] = up->getZ();

    cross(d,upA,res);

    switch (c){
        case 'w':
            position->setX(x+d[0]);
            position->setY(y+d[1]);
            position->setZ(z+d[2]);
            break;
        case 's':
            position->setX(x-d[0]);
            position->setY(y-d[1]);
            position->setZ(z-d[2]);
            break;
        case 'a':
            position->setX(x-res[0]);
            position->setY(y-res[1]);
            position->setZ(z-res[2]);
            break;
        case 'd':
            position->setX(x+res[0]);
            position->setY(y+res[1]);
            position->setZ(z+res[2]);
            break;
        default:
            break;
    }

    updateLookup();
    updateUp();
}

void Camera::thirdperson(){
    this->third = this->third == 1 ? 0 : 1;
}

void Camera::draw(){
    if (third){
        glPushMatrix();
        glTranslatef(position->getX(),position->getY(),position->getZ());

        //glRotatef((alpha*360.0) / (2.0*M_PI),0,1,0);
        //glRotatef((beta*360.0) / (2.0*M_PI),1,0,0);

        if (fig == NULL)
            glutSolidCube(1);
        else fig->draw();
        glPopMatrix();
    }
}

void Camera::setFig(Figure * f){
    fig = f;
}

