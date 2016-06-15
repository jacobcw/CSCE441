// Assignment2.cpp : Defines the entry point for the console application.
//
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//#include <windows.h>
#include <stdlib.h>

static GLfloat spin = 0.0;
GLboolean rightMouseButtonDown, leftMouseButtonDown, middleMouseButtonDown;
GLfloat speed = 1.0;
GLshort mode = 3;
const GLshort points = 1;
const GLshort lines = 2;
const GLshort filled = 3;
GLfloat r=1.0, g=1.0, b=1.0,in=0.0;
GLshort homeX, homeY,deltaX,deltaY,posX;

void init(void) {
	glClearColor(r*in, g*in, b*in, 0.0);
	glLineWidth(5.0);
	glPointSize(5.0);
	glShadeModel(GL_FLAT); 
}

void triangle(void)
{
	switch(mode)
	{
	case filled:
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0+deltaX, 25.0+deltaY, 0.0);
		glVertex3f(-20.0 + deltaX, -10.0 + deltaY, 0.0);
		glVertex3f(20.0 + deltaX, -10.0 + deltaY, 0.0);
		glEnd();
		break;
	case lines:
		glBegin(GL_LINES);
		glVertex2i(0.0 + deltaX,25.0 + deltaY);
		glVertex2i(-20.0 + deltaX, -10.0 + deltaY);
		glVertex2i(20.0 + deltaX, -10.0 + deltaY);
		glVertex2i(0.0 + deltaX,25.0 + deltaY);
		glVertex2i(-20.0 + deltaX,-10.0 + deltaY);
		glVertex2i(20.0 + deltaX,-10.0 + deltaY);
		glEnd();
		break;
	case points:
		glBegin(GL_POINTS);
		glVertex2i(0.0 + deltaX, 25.0 + deltaY);
		glVertex2i(-20.0 + deltaX, -10.0 + deltaY);
		glVertex2i(20.0 + deltaX, -10.0 + deltaY);
		glEnd();
		break;
	default:
		break;
	}
}

void display(void) {
	glClearColor(r*in, g*in, b*in, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix(); 
	glRotatef(-spin, 0.0, 0.0, 1.0); 
	glColor4f(1.0,0.0,0.0,0.0);  
	triangle();
	glTranslatef(deltaX,deltaY,0.0);
	glPopMatrix();  
	glutSwapBuffers(); 
}

void spinDisplay(void) {
	if (leftMouseButtonDown) {
		spin = spin + speed;
		if (spin > 360.0)
			spin = spin - 360.0;
		glutPostRedisplay();
	}
	else if (rightMouseButtonDown) {
		spin = spin - speed;
		if (spin < 0)
			spin = spin + 360.0;
		glutPostRedisplay();
	}
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	glOrtho(-200.0, 200.0, -200.0, 200.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity();
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		mode = points;
		break;
	case '2':
		mode = lines;
		break;
	case'3':
		mode = filled;
		break;
	case 'c':
		r = 0.0; g = 0.1; b = 0.1;
		break;
	case 'm':
		r = 0.1; g = 0.0; b = 0.1;
		break;
	case 'y':
		r = 0.1; g = 0.1; b = 0.0;
		break;
	case 'w':
		r = 0.1; g = 0.1; b = 0.1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) { 
	homeX = x; homeY = y;
	switch (button) { 
	case GLUT_LEFT_BUTTON: 
		leftMouseButtonDown = (state == GLUT_DOWN);
		glutIdleFunc(spinDisplay);   
		break;
	case GLUT_RIGHT_BUTTON:  
		rightMouseButtonDown = (state == GLUT_DOWN);
		glutIdleFunc(spinDisplay); 
		break;
	case GLUT_MIDDLE_BUTTON:
		middleMouseButtonDown = (state == GLUT_DOWN);
		homeX = x; homeY = y;
		glutPostRedisplay();
    default:
		break;
	}

}

void mouseMove(int x, int y)
{

	if (leftMouseButtonDown || rightMouseButtonDown)
	{
		speed = abs(y - homeY) / 100 + 1;
			posX = (x - homeX) / 100;
			in = posX;
	}
	else if (middleMouseButtonDown)
	{
		deltaX = (x - homeX)/10;
		deltaY = -(y - homeY)/10;
		glutPostRedisplay();
	}

}

/* *  Request double buffer display mode. 
*  Register mouse input callback functions */
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Jacob Wallace - Assignment 2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutMainLoop();
	return 0;
}


