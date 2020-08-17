

#include "../headers/Transformation.h"

Transformation::Transformation(){}

Transformation::~Transformation() {};


Transformation* Transformation::clone() {
    return new Transformation();
}

void Transformation::perform() {
}