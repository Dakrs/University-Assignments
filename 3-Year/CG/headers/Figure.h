

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#ifndef __FIGURES__H
#define __FIGURES__H

#include <vector>
#include "Point.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Transformation.h"
#include "Material.h"
#ifdef __APPLE__
#include "/usr/local/include/IL/il.h"
#else
#include <IL/il.h>
#endif



class Figure {
    GLuint buffer[3]; // 0 é para pontos
    int n_vertex;
	unsigned int texture;
	vector<Transformation*> * transformacoes;
	vector<Material*> * materials;
	int errors[3];

    public:
        Figure(vector<Point*> * v,vector<Transformation*> &trans,vector<Point*> * normal,vector<Material*> *  mat,vector<Point *> * texturepoint,const char * texturefile);
        void draw();
        virtual ~Figure();
        void loadImage(const char * texturefile);
};

void cleanVector(vector<Point*> * limpar);


#endif
