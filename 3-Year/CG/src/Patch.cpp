

#include "../headers/Patch.h"


Patch::Patch() {
    control_points = new vector<Point*>();
}

vector<Point *> * Patch::getControlPoints(){
    return control_points;
}

void Patch::addControlPoint(Point * p){
    control_points->push_back(p);
}

Patch::~Patch(){
    delete(control_points);
}

Point * Patch::pointAt(int i){
    return control_points->at(i);
}