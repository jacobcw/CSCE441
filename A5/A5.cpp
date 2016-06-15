#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <vector>
#include <math.h>

#include "structs.h"

using namespace std;

/******************************************************************
	Notes:
	This is the same utility as in the earlier homework assignment.
	Image size is 400 by 400 by default.  You may adjust this if
		you want to.
	You can assume the window will NOT be resized.
	Call clearFramebuffer to clear the entire framebuffer.
	Call setFramebuffer to set a pixel.  This should be the only
		routine you use to set the color (other than clearing the
		entire framebuffer).  drawit() will cause the current
		framebuffer to be displayed.
	As is, your ray tracer should probably be called from
		within the display function.  There is a very short sample
		of code there now.  You can replace that with a call to a
		function that raytraces your scene (or whatever other
		interaction you provide.
	You may add code to any of the subroutines here,  You probably
		want to leave the drawit, clearFramebuffer, and
		setFramebuffer commands alone, though.
  *****************************************************************/

#define ImageW 400
#define ImageH 400
#define pi 3.1415926535897

float ka = 0.2; 
int ks = 25; 
float accuracy = 0.00001;
float theta = 270.0;
float camY = 5;
float r = 10;
float refl = 0.0;
vector<Object*> objects;
vector<Light*> lights;

float framebuffer[ImageH][ImageW][3];

void drawit(void)
{
   glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);
   glFlush();
}

void clearFramebuffer()
{
	int i,j;

	for(i=0;i<ImageH;i++) {
		for (j=0;j<ImageW;j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

void setFramebuffer(int x, int y, float R, float G, float B)
{
	if (R<=1.0)
		if (R>=0.0)
			framebuffer[x][y][0]=R;
		else
			framebuffer[x][y][0]=0.0;
	else
		framebuffer[x][y][0]=1.0;
	if (G<=1.0)
		if (G>=0.0)
			framebuffer[x][y][1]=G;
		else
			framebuffer[x][y][1]=0.0;
	else
		framebuffer[x][y][1]=1.0;
	if (B<=1.0)
		if (B>=0.0)
			framebuffer[x][y][2]=B;
		else
			framebuffer[x][y][2]=0.0;
	else
		framebuffer[x][y][2]=1.0;
}

vector<float> fillIntersect(Vector3 p,Vector3 dir,vector<float> v)
{
	for (int index = 0; index < objects.size(); index++)
		v.push_back(objects[index]->findIntersection(p,dir));
	return v;
}

int findClosest(vector<float> v) {
	float temp=100.0; int obj=-1;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] > 0 && v[i] < temp) {
			temp = v[i];
			obj = i;
		}
	}
	if(obj<0) return -1;
	else return obj;

}

Color shading(Vector3 p, Vector3 v, Color tempColor, Color final_color,Vector3 norm)
{
	for (int i = 0; i < lights.size(); i++) {
		Vector3 lightDir = (lights[i]->pos+(p.neg())).normalize();
		if (norm.dot(lightDir) > 0) {
			bool shadowed = false;
			//Ray shadowRay (p, lightDir);
			vector<float> shadow;
			shadow = fillIntersect(p,lightDir,shadow);
			for (int j = 0; j < shadow.size(); j++) {
				if (shadow[j] > accuracy && shadow[j] <= lightDir.magnitude()) {
					shadowed = true;
				}
			}
			if (shadowed == false) {
				final_color = final_color+(tempColor*(lights[i]->color)*(norm.dot(lightDir)));
				if (tempColor.s > 0 && tempColor.s <= 1) {
					Vector3 reflDir = (v.neg()+(((norm*(norm.dot(v.neg())))+(v))*2)).normalize();
					if (reflDir.dot(lightDir) > 0) {
						float spec = pow(reflDir.dot(lightDir), ks);
						final_color = final_color+(lights[i]->color*(spec*tempColor.s));
					}
				}
				
			}
			
		}
	}
	return final_color;
}

Color ambientCalc(Color c) 
{
	return c*ka;
}

Color specularCalc(Vector3 p, Vector3 v, Color tempColor, Vector3 norm)
{
	Color finalColor = ambientCalc(tempColor);
	if (tempColor.s > 0 && tempColor.s <= 1) {
		Vector3 reflDir = ((v.neg()+((norm*(norm.dot(v.neg())))+v))*2).normalize();
		//Ray rayRefl (p, reflDir);
		vector<float> reflections;
		reflections = fillIntersect(p,reflDir,reflections);
		int clReflObj = findClosest(reflections);
		if (clReflObj != -1) {
				Vector3 reflPos = p+(reflDir*(reflections[clReflObj]));
				Color tempColor2 = objects[clReflObj]->getColor();
				Vector3 norm2 = objects[clReflObj]->getNormalAt(p);
				Color reflColor = specularCalc(reflPos, reflDir, tempColor2, norm2);
				finalColor = finalColor+(reflColor*(tempColor.s));
		}
	}
	return finalColor;

}

// Gets color at each point in scene
Color colorCalculation(Vector3 p, Vector3 v, int close) {
	
	Color tempColor = objects[close]->getColor();
	Vector3 norm = objects[close]->getNormalAt(p);

	Color final_color = specularCalc(p,v,tempColor,norm);
	final_color = shading(p,v,tempColor,final_color,norm);

	return final_color;
}

void display(void){

	
	Vector3 campos (r*cos(theta*pi/180), camY, r*sin(theta*pi/180));
	Vector3 look_at (0, 0, 0);
	Camera cam (campos, look_at);
	
	// Color for lights
	Color white 	(1.0, 1.0, 1.0, 0.0);
	Color red 		(1.0, 0.0, 0.0, 0.0);
	
	// Color for Objects
	Color yellow	(1.0, 1.0, 0.0, 0.25);
	Color black		(0.0, 0.0, 0.0, 1.0);
	Color cyan		(0.0, 1.0, 1.0, 0.75);
	Color magenta 	(1.0, 0.0, 1.0, 0.5);
	Color gray		(0.5, 0.5, 0.5, 0.0);
	
	// Direct Lights
	Vector3 light1pos(0,10,0);
	Vector3 light2pos(10,10,10);
	Light* light1 = new Light(light1pos , white);
	Light* light2 = new Light(light2pos , red);	
	lights.push_back(light1);
	lights.push_back(light2);

	// Scene Objects
	Plane* plane = new Plane (Vector3(0,1,0), 0, gray);
	Sphere* sphere1 = new Sphere (Vector3(2.5, 1.0, 2.5)	, 1	, black);
	Sphere* sphere2 = new Sphere (Vector3(-2.5, 1.0, 2.5)	, 1	, cyan);
	Sphere* sphere3 = new Sphere (Vector3(-2.5, 1.0, -2.5)	, 1	, yellow);
	Sphere* sphere4 = new Sphere (Vector3(2.5, 1.0, -2.5)	, 1	, magenta);
	objects.push_back(sphere1);
	objects.push_back(sphere2);
	objects.push_back(sphere3);
	objects.push_back(sphere4);
	objects.push_back(plane);
	
	for (int x = 0; x < ImageW; x++) {
		for (int y = 0; y < ImageH; y++) {
			float xamnt = (x + 0.5)/ImageW;
			float yamnt = ((ImageH - y) + 0.5)/ImageH;
			Vector3 frameVect = (cam.camdir+(cam.camright*(xamnt - 0.5)+(cam.camdown*(yamnt - 0.5)))).normalize();
			//Ray camRay (cam.campos, camDir);
			vector<float> intersections;
			intersections = fillIntersect(campos,frameVect,intersections);
			int closestObj = findClosest(intersections);
			if (closestObj == -1) { setFramebuffer(y,x,0,0,0); }
			else{
				Vector3 p = campos+(frameVect*(intersections[closestObj]));
				Color trace = colorCalculation(p,frameVect, closestObj);
				setFramebuffer(y,x,trace.r,trace.g,trace.b);
			}
				
		}
	}

	drawit();
}

void init(void)
{
	clearFramebuffer();
}

int main (int argc, char *argv[]) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(ImageW,ImageH);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Jacob Wallace - Homework 5");
	init();	
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}