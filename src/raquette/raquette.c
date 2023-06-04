#include "stb_image.h"
#include "3D_tools.h"

#include "raquette.h"

static const float GL_VIEW_SIZE = 20.;

extern double obstacleSpeed;

Ball balle;
double xpos, ypos;

/*
* Dessine un carré non rempli
*/
void drawUnfilledSquare() { // On dessine un carré vide en dessinant 4 lignes
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	glBegin(GL_QUADS);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0.5, -0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(-0.5, 0.5);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

/*
* Dessine la raquette à la bonne position
*/
void drawRaquette() { // On dessine la raquette à partir du carré vide là où se trouve la souris
	glPushMatrix();
		glColor3f(1,1,1);
		glScalef(1./320,1./320,1.);
		glTranslatef(xpos-640, -(ypos-360), -2.);
		glScalef(160.,160.,1.);
		drawUnfilledSquare();
	glPopMatrix();
}

/*
* Met à jour la position de la balle
*/
void moveBall() {
	if (balle.sticky) { // Si sticky = 1, la balle est dessinée là où se trouve la souris, devant la raquette
		balle.posX = xpos;
		balle.posY = ypos;
		balle.posZ = -0.5;
	}
	else { // Sinon, sa position est déterminée en fonction de sa position précédente et de sa vitesse
		balle.posX += balle.speeX;
		balle.posY += balle.speeY;
		balle.posZ += balle.speeZ+obstacleSpeed;
	}
}

/*
* Dessine la balle à la bonne position
*/
void drawBall(GLuint textures) { // On dessine la balle
	if (balle.posZ <= 0) {
		glPushMatrix();
			glColor3f(1.,1.,1.);
			
			glScalef(1./1280,1./1280,1./1280);
			glTranslatef(balle.posX-640, -(balle.posY-360), balle.posZ*1280);
			glScalef(120.,120.,120.);
			drawSquareTexture(textures);
		glPopMatrix();
	}
}
