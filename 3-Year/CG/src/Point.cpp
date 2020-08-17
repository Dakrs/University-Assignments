#include "../headers/Point.h"

Point::Point(float a,float b, float c){
	x = a;
	y = b;
	z = c;
	type = 0;
}

Point::Point(float a, float b){
	x = a;
	y = b;
	type = 1;
}

float Point::getX() {
	return x;
}

float Point::getY(){
	return y;
}

float Point::getZ(){
	return z;
}

void Point::setX(float s) {
	x = s;
}

void Point::setY(float s) {
	y = s;
}
void Point::setZ(float s) {
	z = s;
}

void Point::get(float * s){
    s[0] = x;
    s[1] = y;
    s[2] = z;
}

string Point::to_String(){
	stringstream ss;

	if (type == 0)
		ss << to_string(x) << " " << to_string(y) << " " << to_string(z) << " ";
	else ss << to_string(x) << " " << to_string(y) << " ";

	return ss.str();
}

Point::~Point(){}
