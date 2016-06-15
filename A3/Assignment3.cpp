#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
#include<math.h>
#include<cstdlib>
#include<map>
#include<algorithm>
#include<string>
#include <GL/glut.h>

#define W 500
#define H 500

const int maxPoints = 10;
const int maxShapes = 10;

using namespace std;

class Point {
private:
	int x,y;
public:
	Point(int x, int y) : x(x), y(H - 1 - y) {}
	int getX() { return this->x;}
	int getY() { return y; }
};
class Color {
public:
	double r, g, b;
	Color() {
		while (true) {
			r = ((double)rand() / (RAND_MAX));
			g = ((double)rand() / (RAND_MAX));
			b = ((double)rand() / (RAND_MAX));

			if (r > 0.2 && g > 0.2 && b > 0.2) {
				break;
			}
		}
	}
	Color(double r, double g, double b) :r(r), g(g), b(b) {}
	void draw(Point &point, float(&framebuffer)[H][W][3]) {
		int y = point.getY();
		int x = point.getX();
		framebuffer[y][x][0] = r;
		framebuffer[y][x][1] = g;
		framebuffer[y][x][2] = b;
	}
	void draw(int x, int y, float(&framebuffer)[H][W][3]) {
		framebuffer[y][x][0] = r;
		framebuffer[y][x][1] = g;
		framebuffer[y][x][2] = b;
	}
};
class Edge {
private:
	Point a, b;
	int minY, maxY;
	int xCurr;
	double xIncr;
	double yInt;
	bool horiz;
public:
	Edge(Point a, Point b) :a(a), b(b) {
		maxY = (a.getY() >= b.getY()) ? a.getY() : b.getY();
		minY = (a.getY() <= b.getY()) ? a.getY() : b.getY();
		if (minY == a.getY()) { xCurr = a.getX(); }
		else { xCurr = b.getX(); }

		int y_diff = (b.getY() - a.getY());
		int x_diff = (b.getX() - a.getX());

		if (x_diff == 0) {xIncr = 0; }
		else { xIncr = ((double)y_diff / (double)x_diff);}

		horiz = (y_diff == 0) ? true : false;
		yInt = a.getY() - xIncr * a.getX();
	}
	int getXIncr(int y) { return (y - yInt) / xIncr; }
	bool is_horizontal_line() { return horiz; }
	int getMaxY() { return maxY; }
	int getMinY() { return minY; }
};
class EdgeSort {
	int y;
public:
	EdgeSort(int y) : y(y) {}
	bool operator()(Edge e1, Edge e2) const {
		return e1.getXIncr(y) < e2.getXIncr(y);
	}
};
class EdgeTable {
private:
	multimap<int, Edge> edgeTable;
public:
	EdgeTable() {}
	EdgeTable(vector<Edge> edges) {
		for (auto &edge : edges) {
			// skip all horizontal lines, not needed
			if (edge.is_horizontal_line()) { continue; }
			edgeTable.insert(pair<int, Edge>(edge.getMinY(), edge));
		}
	}
	void active(int y, vector<Edge> &activeEdge) {
		int count = edgeTable.count(y);
		if (count > 0) {
			multimap<int, Edge>::const_iterator itr = edgeTable.find(y);
			for (int i = 0; i < count; ++i) {
				activeEdge.push_back((*itr).second);
				++itr;
			}
			// list has been changed,
			// sort edges so that concave edges can be drawn
			sort(activeEdge.begin(), activeEdge.end(), EdgeSort(y));
		}
	}

};
class Clip {
private:
	Color c;
	bool clipMode;
	int endX, endY;
	int startX, startY;
public:
	Clip() :
		c(1.0, 1.0, 1.0) {
		clipMode = false;
		reset();
	}
	bool mode() { return clipMode; }
	void toggle() { clipMode = !clipMode; }
	void reset() {
		startX = 0;
		startY = 0;
		endX = W - 1;
		endY = H - 1;
	}
	void start(int x, int y) {
		startY = H - 1 - y;
		startX = x;
		endX = x + 1;
		endY = startY + 1;
	}
	void end(int x, int y) {
		endX = (x > 0 && x < W) ? x : endX;
		int new_y = H - 1 - y;
		endY = (new_y > 0 && new_y < H) ? new_y : endY;
	}

	int getMinY() { return (startY <= endY) ? startY : endY; }
	int getMaxY() { return (startY >= endY) ? startY : endY; }
	int getMinX() { return (startX <= endX) ? startX : endX; }
	int getMaxX() { return (startX >= endX) ? startX : endX; }
	void draw(float(&framebuffer)[H][W][3]) {
		if (clipMode) {
			for (int x = getMinX(); x < getMaxX(); ++x) {
				c.draw(x, getMinY(), framebuffer);
				c.draw(x, getMaxY(), framebuffer);
			}
			for (int y = getMinY(); y < getMaxY(); ++y) {
				c.draw(getMinX(), y, framebuffer);
				c.draw(getMaxX(), y, framebuffer);
			}
		}
	}
};
class Shape {
private:
	Color c;
	vector<Point> points;
	EdgeTable edgeTable;
	vector<Edge> activeEdge;
	int yLow, yHigh;
	bool finished;
	void checkY(Point p) {
		if (p.getY() < yLow) { yLow = p.getY(); }
		if (p.getY() > yHigh) { yHigh = p.getY(); }
	}
public:
	Shape() {
		yLow = H;
		yHigh = 0;
		finished = false;
	}
	int size() { return points.size(); }
	void addPoint(int x, int y) {
		if (points.size() >= (maxPoints - 1)) {
			cout << "Must Right-Click" << endl;
			return;
		}
		Point p(x, y);
		checkY(p);
		points.push_back(p);
	}
	void addLast(int x, int y) {
		Point p(x, y);
		checkY(p);
		points.push_back(p);

		vector<Edge> edges;
		for (int i = 0; i < points.size() - 1; ++i) {
			Edge e(points[i], points[i + 1]);
			edges.push_back(e);
		}
		Edge e(points[0], points[points.size() - 1]);
		edges.push_back(e);

		EdgeTable et(edges);
		edgeTable = et;
		finished = true;
	}
	void draw(float(&framebuffer)[H][W][3]) {
		if (!finished) {
			for (auto &point : points) {
				c.draw(point.getX() - 1, point.getY() - 1, framebuffer);
				c.draw(point.getX() - 1, point.getY(), framebuffer);
				c.draw(point.getX(), point.getY() - 1, framebuffer);
				c.draw(point.getX() + 1, point.getY() + 1, framebuffer);
				c.draw(point.getX() + 1, point.getY(), framebuffer);
				c.draw(point.getX(), point.getY() + 1, framebuffer);
				c.draw(point.getX() - 1, point.getY() + 1, framebuffer);
				c.draw(point.getX() + 1, point.getY() - 1, framebuffer);
				c.draw(point, framebuffer);
			}
		}
	}
	void clearActive() {
		activeEdge.clear();
	}
	void fill(int y, float(&framebuffer)[H][W][3], Clip &cw) {
		edgeTable.active(y, activeEdge);

		for (int i = activeEdge.size() - 1; i >= 0; --i) {
			if (activeEdge[i].getMaxY() <= y) {
				activeEdge.erase(activeEdge.begin() + i);
			}
		}

		for (int i = 0; i < activeEdge.size(); i += 2) {
			int x_1 = activeEdge[i].getXIncr(y);
			int x_2 = activeEdge[i + 1].getXIncr(y);
			int max_x = (x_1 >= x_2) ? x_1 : x_2;
			int min_x = (x_1 <= x_2) ? x_1 : x_2;
			if (max_x > W || max_x < 0 || min_x > W || min_x < 0) { continue; }
			if (min_x < cw.getMinX() && max_x < cw.getMinX()) { continue; }
			if (min_x > cw.getMaxX() && max_x > cw.getMaxX()) { continue; }
			if (min_x < cw.getMinX()) { min_x = cw.getMinX(); }
			if (max_x > cw.getMaxX()) { max_x = cw.getMaxX(); }
			for (int x = min_x; x < max_x; ++x) {
				if (cw.getMinY() <= y && y <= cw.getMaxY()) {
					c.draw(x, y, framebuffer);
				}
			}
		}
		cw.draw(framebuffer);
	}
};
class Shapes {
private:
	bool newShape;
	vector<Shape> shapes;
public:
	Shapes() {
		newShape = true;
	}
	void addPoint(int x, int y) {
		if (newShape) {
			if (shapes.size() >= maxShapes) {
				cout << "Max Shapes Allowed" << endl;
				return;
			}
			newShape = false;
			Shape p;
			shapes.push_back(p);
		}
		shapes.back().addPoint(x, y);
	}
	void addLast(int x, int y) {
		if (shapes.back().size() < 2) {
			cout << "Atleast 3 Points" << endl;
			return;
		}
		if (newShape) {
			cout << "Must Left-Click" << endl;
			return;
		}
		newShape = true;
		shapes.back().addLast(x, y);
	}
	void draw(float(&framebuffer)[H][W][3]) {
		for (int i = 0; i < shapes.size(); ++i) {
			shapes[i].draw(framebuffer);
		}
	}
	void fill(float(&framebuffer)[H][W][3], Clip &cw) {
		// fill the buffer
		for (int y = 0; y < H; ++y) {
			for (int i = 0; i < shapes.size(); ++i) {
				shapes[i].fill(y, framebuffer, cw);
			}
		}
		// cleanup any leftover elements in the edge list
		for (int i = 0; i < shapes.size(); ++i) {
			shapes[i].clearActive();
		}
	}
};

float framebuffer[H][W][3];
Shapes ss;
Clip cc;

// Draws the scene
void drawit(void) {
	glDrawPixels(H, W, GL_RGB, GL_FLOAT, framebuffer);
	glFlush();
}

// Clears framebuffer to black
void clearFramebuffer() {
	int i, j;

	for (i = 0; i < H; i++) {
		for (j = 0; j < W; j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

void setFramebuffer(int x, int y, float R, float G, float B) {
	// changes the origin from the lower-left corner to the upper-left corner
	y = H - 1 - y;
	if (R <= 1.0)
		if (R >= 0.0)
			framebuffer[y][x][0] = R;
		else
			framebuffer[y][x][0] = 0.0;
	else
		framebuffer[y][x][0] = 1.0;
	if (G <= 1.0)
		if (G >= 0.0)
			framebuffer[y][x][1] = G;
		else
			framebuffer[y][x][1] = 0.0;
	else
		framebuffer[y][x][1] = 1.0;
	if (B <= 1.0)
		if (B >= 0.0)
			framebuffer[y][x][2] = B;
		else
			framebuffer[y][x][2] = 0.0;
	else
		framebuffer[y][x][2] = 1.0;
}

void display(void) {
	clearFramebuffer();
	ss.draw(framebuffer);
	ss.fill(framebuffer, cc);
	drawit();
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (!cc.mode()) {
			if (button == GLUT_LEFT_BUTTON) { ss.addPoint(x, y); }
			if (button == GLUT_RIGHT_BUTTON) { ss.addLast(x, y); }
		}
		else { cc.start(x, y); }
		glutPostRedisplay();
	}

}

void mouseMove(int x, int y) {
	if (cc.mode()) {
		cc.end(x, y);
		glutPostRedisplay();
	}
}

void keyPress(unsigned char key, int x, int y) {
	switch ((char)key) {
	case 'c':
		cc.toggle();
		cc.reset();
		if (cc.mode()) { cout << "Clipping ON" << endl; }
		else { cout << "Clipping OFF" << endl; }
		break;
	default:
		break;
	}
	glutPostRedisplay();
	return;
}

void init(void) { clearFramebuffer(); }

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Jacob Wallace | Assignment 3");
	init();
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyPress);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
