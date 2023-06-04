#include "stb_image.h"
#include "3D_tools.h"

#include "corridor.h"

/*
* Dessine un carré sur lequel on applique une texture
*/
void drawSquareTexture(GLuint textures) {
	glEnable(GL_TEXTURE_2D);
		
	glBindTexture(GL_TEXTURE_2D, textures);
	
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
	
	glBindTexture(GL_TEXTURE_2D, 0);
		
	glDisable(GL_TEXTURE_2D);
}

/*
* Dessine les quatre murs du couloir
*/
void drawCorridor(GLuint texturesTop, GLuint texturesBottom, GLuint texturesSides) {
	// dessiner mur gauche
	glPushMatrix();
	glTranslatef(-8, 0., -GL_VIEW_SIZE);
	glScalef(4.5, 9, 85);
	glRotatef(90, 0, 1, 0);
	drawSquareTexture(texturesSides);
	glPopMatrix();

	// dessiner mur droit
	glPushMatrix();
	glTranslatef(8, 0., -GL_VIEW_SIZE);
	glScalef(4.5, 9, 85);
	glRotatef(90, 0, 1, 0);
	drawSquareTexture(texturesSides);
	glPopMatrix();

	// dessiner mur du haut
	glPushMatrix();
	glTranslatef(0, 4.5, -GL_VIEW_SIZE);
	glScalef(16, 8, 85);
	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	drawSquareTexture(texturesTop);
	glPopMatrix();

	// dessiner mur du bas
	glPushMatrix();
	glTranslatef(0, -4.5, -GL_VIEW_SIZE);
	glScalef(16, 8, 85);
	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	drawSquareTexture(texturesBottom);
	glPopMatrix();
}
