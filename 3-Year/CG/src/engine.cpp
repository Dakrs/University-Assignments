#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#endif


#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "../headers/parser.h"
#include "../headers/Camera.h"
#include "../headers/Light.h"


using namespace tinyxml2;
using namespace std;

#define 	GLUT_WHEEL_UP 		3
#define 	GLUT_WHEEL_DOWN 	4

//variaveis globais
int WINDOW_X = 800;
int WINDOW_Y = 800;
int X_POS = WINDOW_X/2;
int Y_POS = WINDOW_Y/2;

float radius = 40;
GLenum viewMode = GL_POLYGON;

Camera * cam;

vector<Figure*> figs;
vector<Light*> lights;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void draw_figures(){
	vector<Figure*>::iterator it;
	for(it = figs.begin(); it != figs.end(); it++){
		(*it)->draw();
	}
}


void renderLights(){
	vector<Light*>::iterator it;
	for(it = lights.begin(); it != lights.end(); it++){
		(*it)->draw();
	}
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	Point * pos = cam->getPosition();
	Point * look = cam->getLookup();
	Point * up = cam->getUp();
	gluLookAt(pos->getX(), pos->getY(),pos->getZ(),
					look->getX(),look->getY(),look->getZ(),
				up->getX(),up->getY(),up->getZ());
	// End of frame

	renderLights();

	glPolygonMode(GL_FRONT,viewMode);
	draw_figures();
	cam->draw();


	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	switch (c) {
			case 'w':
					cam->move(c,xx,yy);
					glutPostRedisplay();
					break;
			case 's':
					cam->move(c,xx,yy);
					glutPostRedisplay();
					break;
			case 'a':
					cam->move(c,xx,yy);
					glutPostRedisplay();
					break;
			case 'd':
					cam->move(c,xx,yy);
					glutPostRedisplay();
					break;
			case 'j':
					viewMode = GL_LINE;
					glutPostRedisplay();
					break;
			case 'k':
					viewMode = GL_POINT;
					glutPostRedisplay();
					break;
			case 'l':
					viewMode = GL_FILL;
					glutPostRedisplay();
					break;

			case 'u':
					cam->thirdperson();
					break;
			default:
					break;
	}

}


void processSpecialKeys(int key, int xx, int yy) {

}

void regPos(int x, int y){
	X_POS = x;
	Y_POS = y;
}



void mouse(int button, int state, int x, int y)
{
	// Used for wheels, has to be up
	if (state == GLUT_UP )	{

		if ( button == GLUT_WHEEL_UP ){
			radius *= 0.95;
		}
		else if( button == GLUT_WHEEL_DOWN ){
			radius *= 1.05;
		}
		glutPostRedisplay();
	}
}




void activeMotion(int x, int y){
	float alpha = ((float)(x - X_POS)/WINDOW_X)*M_PI;
	float beta = ((float)(y - Y_POS)/WINDOW_Y)*M_PI;
	regPos(x,y);
	cam->updateCamera(alpha,beta);
	glutPostRedisplay();
}

void initGL(){

#ifndef __APPLE__
	glewInit();
#endif
	ilInit();

	//  OpenGL settings

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WINDOW_X,WINDOW_Y);
	glutCreateWindow("CG@DI-UM");

    initGL();


	glClearColor(0,0,0,1) ;
	glClear(GL_COLOR_BUFFER_BIT);




	if(argc < 2){
	     cout << "Input not given!" << endl;
	     return 0;
	}

	else {
		cam = new Camera();
	    figs = * (parseXML(argv[1],&lights)); // Read XML File
	    if (argc == 3){
			vector<Light * > aux1;
	    	vector<Figure * > aux = * (parseXML(argv[2],&aux1));
	    	cam->setFig(aux[0]);
	    }
	}
// Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutPassiveMotionFunc(regPos);
	glutMotionFunc(activeMotion);
	glutMouseFunc(mouse);


// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
