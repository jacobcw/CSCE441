struct Insect
{
	float r,l,legL,hip;
	float theta, phi;

	void head(float r){
		glColor3f(1,0,0);
		glTranslatef(-4,0,0);
		glRotatef(90,0,1,0);
		glutSolidCone(r/2,4.5,100,100);
		glutSolidSphere(r,100,100);
		glTranslatef(4,0,0);
	}

	void body(float l){
		glColor3f(0,1,0);
		glTranslatef(l/4,0,0);
		glutSolidCube(l/2);
		glTranslatef(-l/2,0,0);
		glutSolidCube(l/2);
		glTranslatef(l/4,0,0);
	}

	void leg(float l, float r){
		glColor3f(0,0,1);
		glutSolidSphere(r,100,100);
		glTranslatef(0,0,-r-l/2);
		glutSolidCube(l/3);
		glTranslatef(0,0,l/3);
		glutSolidCube(l/3);
		glTranslatef(0,0,-2*(l/3));
		glutSolidCube(l/3);
		glTranslatef(0,0,-l/6-r);
		glutSolidSphere(r,100,100);
		glRotatef(270,1,0,0);
		glutSolidCone(r,l*2,100,100);
		glRotatef(-270,1,0,0);
		glTranslatef(0,0,2*r+l);

	}

	void display(float theta,float phi, float scale, float RotH, float RotV) {
		glPushMatrix();
		glScalef(scale,scale,1); // scale
		glRotatef(RotH, 0,1,0);
		glRotatef(RotV, 1,0,0);

		body(l);

		glTranslatef(-l/3,0,-l/4);
		glRotatef(-45,1,0,0);
		leg(legL,hip);
		glTranslatef(l/3,0,0);
		leg(legL,hip);
		glTranslatef(l/3,0,0);
		leg(legL,hip);
		glRotatef(45,1,0,0);
		glRotatef(180,0,1,0);
		glTranslatef(0,0,-l/2);
		glRotatef(-45,1,0,0);
		leg(legL,hip);
		glTranslatef(l/3,0,0);
		leg(legL,hip);
		glTranslatef(l/3,0,0);
		leg(legL,hip);
		glRotatef(45,1,0,0);
		glTranslatef(l/3,0,l/4);

		glRotatef(180,0,1,0);
		glTranslatef(l/3,0,0);
		glRotatef(theta,0,1,0);
		glRotatef(phi,0,0,1);
		head(r);
		glRotatef(-theta,0,1,0);
		glRotatef(-phi,0,0,1);
		glRotatef(-90,0,1,0);
		glTranslatef(l/3,0,0);
		
		glScalef(-scale,-scale,1);
		glPopMatrix();
	}

	Insect(float theta, float phi): theta(theta), phi(phi) {
		r = 2; l = 8; legL = 4; hip = 1;
	}

	Insect() {}

};