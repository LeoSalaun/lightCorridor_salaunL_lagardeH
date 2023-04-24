#include "stb_image.h"
#include "3D_tools.h"

#include "raquette.h"

static const float GL_VIEW_SIZE = 20.;

Ball balle;
double xpos, ypos;

void drawUnfilledSquare() { // On dessine un carré vide en dessinant 4 lignes
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-0.5, -0.5);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(0.5, -0.5);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(0.5, 0.5);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-0.5, 0.5);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void drawRaquette() { // On dessine la raquette à partir du carré vide là où se trouve la souris
	glPushMatrix();
		glColor3f(1,1,1);
		glScalef(2.5,2.5,2.5);
		glTranslatef(xpos/78-8.2, -ypos/78+4.7, 0.);
		drawUnfilledSquare();
	glPopMatrix();
}

void moveBall() {
	if (balle.sticky) { // Si sticky = 1, la balle est dessinée là où se trouve la souris, devant la raquette
		balle.posX = xpos/30-21.35;
		balle.posY = -ypos/30+12.;
		balle.posZ = -1. - GL_VIEW_SIZE;
	}
	else { // Sinon, sa position est déterminée en fonction de sa position précédente et de sa vitesse
		balle.posX += balle.speeX;
		balle.posY += balle.speeY;
		balle.posZ += balle.speeZ;
	}
}

void drawBall() { // On dessine la balle
	if (balle.posZ >= -40 && balle.posZ <= 0) {
		glPushMatrix();
			glColor3f(/*1.0+balle.posZ/40*/ 1,0,0);
			glTranslatef( balle.posX , balle.posY , balle.posZ );
			drawSphere();
		glPopMatrix();
	}
}
