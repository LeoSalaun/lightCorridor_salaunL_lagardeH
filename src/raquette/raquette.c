#include "stb_image.h"
#include "3D_tools.h"

#include "raquette.h"

static const float GL_VIEW_SIZE = 20.;

extern double obstacleSpeed;

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
		glScalef(1./320,1./320,1.);
		glTranslatef(xpos-640, -(ypos-360), -2.);
		glScalef(160.,160.,1.);
		drawUnfilledSquare();
	glPopMatrix();
}

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
		//printf("%f %f\n",balle.posX,balle.posY);
	}
}

void drawBall(GLuint textures) { // On dessine la balle
	if (/*balle.posZ >= -10 && */balle.posZ <= 0) {
		glPushMatrix();
			glColor3f(1.0+balle.posZ/12,1.0+balle.posZ/12,1.0+balle.posZ/12);
			//glTranslatef( (balle.posX-640)/720 , -(balle.posY-360)/720 , balle.posZ);
			//glScalef(0.075,0.075,0.075);
			//glScalef(1./320,1./320,1.);
			//glTranslatef(balle.posX-640, -(balle.posY-360), balle.posZ);
			//glScalef(80.,80.,80./320);
			glScalef(1./1280,1./1280,1./1280);
			glTranslatef(balle.posX-640, -(balle.posY-360), balle.posZ*1280);
			glScalef(120.,120.,120.);
			//drawSphere();
			drawSquareTexture(textures);
		glPopMatrix();
	}
}
