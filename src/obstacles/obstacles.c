#include <time.h>
#include <math.h>
#include "stb_image.h"
#include "3D_tools.h"

#include "obstacles.h"

double obstacleSpeed;
double corridorBorderPos[4];
Obstacle obstacles[NB_OBSTACLES];

int nbObstacles = NB_OBSTACLES;

/*
* Dessine les rectangle blancs sur lesquels sont situés les obstacles
*/
void drawCorridorBorder()
{
	glPushMatrix();
	glScalef(12. / 9., 0.75, 1.);
	for (int i = 0; i < 4; i++)
	{
		corridorBorderPos[i] += obstacleSpeed;
		if (corridorBorderPos[i] >= -1)
		{ // Si la position d'un rectangle dépasse la caméra, alors elle est remise au départ pour donner l'impression que l'on en traverse une infinité
			corridorBorderPos[i] -= OBSTACLE_LENGTH;
		}
		glColor3f(1.0 + corridorBorderPos[i] / (1. * OBSTACLE_LENGTH), 1.0 + corridorBorderPos[i] / (1. * OBSTACLE_LENGTH), 1.0 + corridorBorderPos[i] / (1. * OBSTACLE_LENGTH));
		glPushMatrix();
		glTranslatef(0.0, 0.0, corridorBorderPos[i]);
		drawUnfilledSquare();
		glPopMatrix();
	}
	glPopMatrix();
}

/*
* Initialise la liste des obstacles
*/
void initObstacle()
{
	srand(time(NULL));
	for (int i = 0; i < NB_OBSTACLES; i++)
	{
		int wall = rand() % 4;
		switch (wall)
		{ // On détermine aléatoirement si l'obstacle est à droite, à gauche, en haut ou en bas
		case 0:
			obstacles[i].wall = 'b';
			break;
		case 1:
			obstacles[i].wall = 't';
			break;
		case 2:
			obstacles[i].wall = 'l';
			break;
		case 3:
			obstacles[i].wall = 'r';
			break;
		}

		obstacles[i].pos = -OBSTACLE_SPACE * (i + 1); // Position en Z de l'obstacle
		wall = rand() % 2;
		switch (wall)
		{ // On détermine aléatoirement si l'obstacle comblera le tiers ou les deux tiers du couloir
		case 0:
			obstacles[i].size = 1. / 3;
			break;
		case 1:
			obstacles[i].size = 2. / 3;
			break;
		}
	}
}

/*
* Dessine les obstacles
*/
void drawObstacles(GLuint texturesAraignee, GLuint texturesCreeper, GLuint texturesSquelette, GLuint texturesZombie, GLuint texturesFin, int posBalle)
{
	glPushMatrix();
	glScalef(12. / 9., 0.75, 1.);
	for (int i = 0; i < NB_OBSTACLES; i++)
	{
		obstacles[i].pos += obstacleSpeed;
		if (obstacles[i].pos <= 0) // Ne dessine l'obstacle que s'il est devant la caméra
		{
			double lightCamera = 1.0 + obstacles[i].pos / (1. * OBSTACLE_LENGTH), lightBall;
			
			if (obstacles[i].pos - posBalle > 0) {
				lightBall = 0;
			}
			else { // Gère la lumière émise par la balle si elle est devant l'obstacle et si elle en est assez proche
				lightBall = 1. + 1. * (obstacles[i].pos - posBalle) / (1. * OBSTACLE_SPACE);
			}
			
			glColor3f(fmax(lightCamera,lightBall),
			          fmax(lightCamera,lightBall),
			          fmax(lightCamera,lightBall));
				
			glPushMatrix();
				switch (obstacles[i].wall)
				{
				case 'b':
					glScalef(1.0, obstacles[i].size, 1.0);
					glTranslatef(0.0, -1. / pow(3 * obstacles[i].size, 2), obstacles[i].pos);
					break;
				case 't':
					glScalef(1.0, obstacles[i].size, 1.0);
					glTranslatef(0.0, 1. / pow(3 * obstacles[i].size, 2), obstacles[i].pos);
					break;
				case 'l':
					glScalef(obstacles[i].size, 1.0, 1.0);
					glTranslatef(-1. / pow(3 * obstacles[i].size, 2), 0.0, obstacles[i].pos);
					break;
				case 'r':
					glScalef(obstacles[i].size, 1.0, 1.0);
					glTranslatef(1. / pow(3 * obstacles[i].size, 2), 0.0, obstacles[i].pos);
					break;
				}
				if (obstacles[i].size == 1. / 3)
				{
					if (obstacles[i].wall == 't' || obstacles[i].wall == 'b')
					{
						drawSquareTexture(texturesAraignee);
					}
					if (obstacles[i].wall == 'l' || obstacles[i].wall == 'r')
					{
						drawSquareTexture(texturesSquelette);
					}
				}

				else if (obstacles[i].size == 2. / 3)
				{
					if (obstacles[i].wall == 't' || obstacles[i].wall == 'b')
					{
						drawSquareTexture(texturesCreeper);
					}
					if (obstacles[i].wall == 'l' || obstacles[i].wall == 'r')
					{
						drawSquareTexture(texturesZombie);
					}
				}
			glPopMatrix();
		}
	}
	
	// Reproduit le même processus pour le mur de fin
	double lightCamera = 1.0 + (obstacles[NB_OBSTACLES - 1].pos - OBSTACLE_SPACE) / (1. * OBSTACLE_LENGTH),
	       lightBall;
	
	if ((obstacles[NB_OBSTACLES - 1].pos - OBSTACLE_SPACE) - posBalle > 0) {
		lightBall = 0;
	}
	else {
		lightBall = 1.0 + 1. * ((obstacles[NB_OBSTACLES - 1].pos - OBSTACLE_SPACE) - posBalle) / (OBSTACLE_SPACE);
	}
	
	glColor3f(fmax(lightCamera,lightBall),
	          fmax(lightCamera,lightBall),
	          fmax(lightCamera,lightBall));
	glTranslatef(0.0, 0.0, obstacles[NB_OBSTACLES - 1].pos - OBSTACLE_SPACE);
	drawSquareTexture(texturesFin);
	glPopMatrix();
}
