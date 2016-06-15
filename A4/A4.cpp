#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "Insect.h"

Insect i;
float theta = 0,phi = 0;

float translation[2] = {0,0};
float rotation[2] = {0,0};
float scale = 1;
int currentOperation = 0;

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(translation[0],translation[1],0);
	i.display(theta,phi,scale,rotation[0],rotation[1]);
	glTranslatef(-translation[0],-translation[1],0);
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		glutPostRedisplay();
	}
}

void mouseMove(int x, int y) {
	switch (currentOperation) {
	case 0: // rotate
		if (rotation[0] > 0 || rotation[0] < 360) rotation[0] = x;
		if (rotation[1] > 0 || rotation[1] < 360) rotation[1] = y;
		break;
	case 1: //scaling
		if (x > 0 || x < 400) { scale = ((x*4.0)/400); }
		break;
	case 2: // translation
		int locX = x - 200;
		int locY = 200 - y;
		translation[0] = (locX * 10) / 300;
		translation[1] = (locY * 10) / 300;
		break;
	}
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		++currentOperation;
		if (currentOperation == 3) { currentOperation = 0; }
		break;
	case 'd':
		theta++; if(theta>45) theta=45;
		break;
	case 'a':
		theta--; if(theta<-45) theta=-45;
		break;
	case 'w':
		phi++; if(phi>45) phi=45;
		break;
	case 's':
		phi--; if(phi<-45) phi=-45;
		break;
	case 'r':
		theta = 0; phi = 0; translation[0] = 0; translation[1]=0;
	break;
	default: break;
	}
	glutPostRedisplay();
	return;
}

void init(void){
	i = Insect(theta,phi);
	glClearColor(0,0,0,0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20,20,-20,20,-50,50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-20,20,-20,0,0,0,0,1,0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat light[] = {0.4,1.0,1.0,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,light);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Jacob Wallace - Assignment 4");
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(display);

	init();
	glutMainLoop();
	return 0;
}