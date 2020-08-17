#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>
#include <string>
#include <sstream>

using namespace std;

class Point{

	float x;
	float y;
	float z;
	int type;

	public:
		Point(float x,float y,float z);
		Point(float x,float y);
		float getX();
		float getY();
		float getZ();
		void setX(float s);
		void setY(float s);
		void setZ(float s);
		void get(float * s);
		string to_String();
		virtual ~Point();
};

#endif
