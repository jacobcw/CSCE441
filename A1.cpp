
#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
/*
void display(void){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnd();
	glFlush();
	return;
}

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	GLdouble width,height;
	width=800.0;
	height=400.0;

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize((int)800,(int)600);
	glutCreateWindow ("An Example OpenGL Program");
	
	glOrtho(0.0,width,0.0,height,-1.f,1.f);

	glutDisplayFunc(display);

	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(0.0,0.0,0.0);
	glLineWidth(3.0);

	glutMainLoop();

	exit(0);
}
*/
void init (void)
{
    glMatrixMode (GL_PROJECTION);       // Set projection parameters.
    gluOrtho2D (0.0, (GLdouble)900.0, 0.0, (GLdouble)450.0);
    return;
}
void hexagon (void)
{
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);  // Set display-window color to white.
	glClear(GL_COLOR_BUFFER_BIT);
	
    glBegin (GL_LINES);
    	glColor3f (1.0, 1.0, 1.0);      // Set line segment color to white.
        glVertex2i (500, 138);       // Specify line-segment geometry.
        glVertex2i (400, 138);

		glColor3f (0.0, 1.0, 0.0);      // Set line segment color to green.
        glVertex2i (400,138);       // Specify line-segment geometry.
        glVertex2i (350,225);
    
    	glColor3f (1.0, 0.0, 1.0);      // Set line segment color to purple.
        glVertex2i (350,225);       // Specify line-segment geometry.
        glVertex2i (400,312);
    
    	glColor3f (0.8, 0.3, 0.1);      // Set line segment color to orange.
        glVertex2i (400,312);       // Specify line-segment geometry.
        glVertex2i (500,312);
    
    	glColor3f (0.38, 0.2, 0.07);      // Set line segment color to brown.
        glVertex2i (500,312);       // Specify line-segment geometry.
        glVertex2i (550,225);
 
    	glColor3f (0.98, 0.86, 0.05);      // Set line segment color to green.
        glVertex2i (550,225);       // Specify line-segment geometry.
        glVertex2i (500,138);
    glEnd ( );
	glFlush ( );     // Process all OpenGL routines as quickly as possible.

	return;
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);   // Set display mode.
    glutInitWindowPosition (50.0, 100.0);   // Set top-left display-window position.
    glutInitWindowSize (900.0, 450.0);      // Set display-window width and height.
    glutCreateWindow ("Jacob Walace - Assignment 1"); // Create display window.
	init ( );
	glLineWidth(7.0);
	glutDisplayFunc (hexagon);

	glutMainLoop ( );
	exit(0);
}