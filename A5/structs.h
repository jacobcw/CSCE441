#include <math.h>

struct Color {
	double r, g, b, s;
	Color(): r(0.5), g(0.5), b(0.5) {}
	Color(double r, double g, double b, double s): r(r),g(g),b(b),s(s) {}

	double brightness() { return(r + g + b)/3; }
	Color operator+(Color c) { return Color (r + c.r, g + c.g, b + c.b, s); }
	Color operator*(Color c) { return Color (r*c.r, g*c.g, b*c.b, s); }
	Color operator*(double scalar) { return Color (r*scalar, g*scalar, b*scalar, s); }

};

struct Vector3{
	double x, y, z;
	
	double magnitude () { return sqrt((x*x) + (y*y) + (z*z)); }
	Vector3 normalize () {
		double mag = this->magnitude();
		return Vector3 (x/mag, y/mag, z/mag);
	}
	Vector3 neg() { return Vector3 (-x, -y, -z); }
	double dot(Vector3 v) { return x*v.x + y*v.y + z*v.z; }
	Vector3 cross(Vector3 v) { return Vector3 (y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x ); }
	Vector3 operator+(Vector3 v) { return Vector3(x+v.x, y+v.y, z+v.z); }
	Vector3 operator*(double scalar) { return Vector3 (x*scalar, y*scalar, z*scalar); }

	Vector3(): x(0), y(0), z(0) {}
	Vector3(double x,double y,double z): x(x), y(y), z(z) {}
};

struct Ray {
	Vector3 o, dir;
	
	Ray(): o(Vector3(0,0,0)), dir(Vector3(0,1,0)) {}
	Ray (Vector3 o, Vector3 d): o(o), dir(d) {} 
};

struct Camera {
	Vector3 campos, camdir, camright, camdown;
	
	Camera(): campos(Vector3(0,0,0)), camdir(Vector3(0,0,1)), camright(Vector3(0,0,0)), camdown(Vector3(0,0,0)) {}
	Camera (Vector3 p, Vector3 look): campos(p) {
		camdir = Vector3(p.x-look.x,p.y-look.y,p.z-look.z).neg().normalize();
		camright = Vector3(0,1,0).cross(camdir).normalize();
		camdown = camright.cross(camdir);
	}
};

struct Object {
	Object () {}
	virtual Color getColor () { return Color (0.0, 0.0, 0.0, 0); }
	virtual Vector3 getNormalAt(Vector3 intersection_position) { return Vector3 (0, 0, 0); }
	virtual double findIntersection(Vector3 p, Vector3 dir) { return 0; }
};

struct Light {
	Vector3 pos;
	Color color;
	
	Light(): pos(Vector3(0,0,0)), color(Color(1,1,1,0)) {}
	Light (Vector3 p, Color c): pos(p), color(c) {}	
};

struct Sphere : public Object {
	Vector3 center;
	double r;
	Color c;
	
	Sphere(): center(Vector3(0,0,0)), r(1.0), c(Color(0.5,0.5,0.5,0)) {}
	Sphere(Vector3 v, double r, Color c): center(v), r(r), c(c) {}
	
	virtual Color getColor () { return c; }
	virtual Vector3 getNormalAt(Vector3 point) {
		return (point+(center.neg())).normalize();
	}
	
	virtual double findIntersection(Vector3 p, Vector3 dir) {
		// a = 1
		double b = (p+(center.neg())).dot(dir)*2;
		double c = (p+(center.neg())).dot((p+(center.neg())))-pow(r,2);
		if (b*b - 4*c < 0) { return -1;}
		else{
			double root_1 = ((-b - sqrt(b*b - 4*c))/2) - 0.000001;	// find first root
			if (root_1 > 0) { return root_1; }							// return smaller root
			else { return ((sqrt(b*b - 4*c) - b)/2) - 0.000001; }
		}
	}
};

struct Plane : public Object {
	Vector3 normal;
	double d;
	Color c;
	
	Plane(): normal(Vector3(1,0,0)), d(0), c(Color(0.5,0.5,0.5,0)) {}
	Plane (Vector3 n, double d, Color c): normal(n), d(d), c(c) {}
	
	virtual Color getColor () { return c; }
	virtual Vector3 getNormalAt(Vector3 point) { return normal; }
	
	virtual double findIntersection(Vector3 p, Vector3 dir) {
		double a = dir.dot(normal);
		if (a == 0) { return -1; }	// parallel to plane
		else {
			double b = normal.dot(p+((normal*d).neg()));
			return -1*b/a;
		}
	}	
};
