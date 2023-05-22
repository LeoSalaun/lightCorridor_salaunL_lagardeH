#include "stb_image.h"
#include "3D_tools.h"

#include "corridor.h"

void drawSideWall(GLuint textures) {
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

void drawCorridor(GLuint texturesTop, GLuint texturesBottom, GLuint texturesSides) {
	// dessiner mur 9 gauche jaune
	glPushMatrix();
	glTranslatef(-8, 0., -GL_VIEW_SIZE);
	glScalef(4.5, 9, 85);
	glRotatef(90, 0, 1, 0);
	drawSideWall(texturesSides);
	glPopMatrix();

	// dessiner mur 9 droit rouge
	glPushMatrix();
	glTranslatef(8, 0., -GL_VIEW_SIZE);
	glScalef(4.5, 9, 85);
	glRotatef(90, 0, 1, 0);
	drawSideWall(texturesSides);
	glPopMatrix();

	// dessiner mur 16 haut bleu
	glPushMatrix();
	glTranslatef(0, 4.5, -GL_VIEW_SIZE);
	glScalef(16, 8, 85);
	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	drawSideWall(texturesTop);
	glPopMatrix();

	// dessiner mur 16 bas rose
	glPushMatrix();
	glTranslatef(0, -4.5, -GL_VIEW_SIZE);
	glScalef(16, 8, 85);
	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	drawSideWall(texturesBottom);
	glPopMatrix();
}
