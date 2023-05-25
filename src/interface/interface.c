#include "stb_image.h"
#include "3D_tools.h"

#include "interface.h"

Player player;

void drawVies(GLuint textures) {
	glPushMatrix();
		glScalef(0.2,0.2,1.);
		glTranslatef(-4.5,-2,-1.);
		for (int i=0 ; i<player.nbVies ; i++) {
			drawSquareTexture(textures);
			glTranslatef(1.1,0.,0.);
		}
	glPopMatrix();
}

void drawSticky(GLuint textures) {
	glPushMatrix();
		glScalef(0.2,0.2,1.);
		glTranslatef(4.5,-2,-1.);
		drawSquareTexture(textures);
	glPopMatrix();
}

void drawInterface(GLuint texturesVie, GLuint texturesSticky) {
	drawVies(texturesVie);
	if (player.sticky) {
		drawSticky(texturesSticky);
	}
}
