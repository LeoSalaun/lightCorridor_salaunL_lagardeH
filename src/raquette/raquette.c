#include "stb_image.h"
#include "3D_tools.h"

#include "raquette.h"

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
		glTranslatef(xpos/100-5, -ypos/100+4, 0.);
		drawUnfilledSquare();
	glPopMatrix();
}

void drawBall() { // On dessine la balle
	glPushMatrix();
		if (balle.sticky) { // Si sticky = 1, la balle est dessinée là où se trouve la souris, devant la raquette
			balle.posX = xpos*2.5/100-12.5;
			balle.posY = -ypos*2.5/100+10;
			balle.posZ = -1.;
		}
		else { // Sinon, sa position est déterminée en fonction de sa position précédente et de sa vitesse
			balle.posX += balle.speeX;
			balle.posY += balle.speeY;
			balle.posZ += balle.speeZ;
		}
		glColor3f(0.75,0,0);
		glTranslatef( balle.posX , balle.posY , balle.posZ );
		drawSphere();
	glPopMatrix();
}
