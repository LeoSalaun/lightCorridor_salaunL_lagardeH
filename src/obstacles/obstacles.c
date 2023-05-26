#include <time.h>
#include "stb_image.h"
#include "3D_tools.h"

#include "obstacles.h"

double obstacleSpeed;
double corridorBorderPos[4] = {-OBSTACLE_SPACE,-2*OBSTACLE_SPACE,-3*OBSTACLE_SPACE,-4*OBSTACLE_SPACE};
Obstacle obstacles[NB_OBSTACLES];

int nbObstacles = NB_OBSTACLES;

void drawCorridorBorder() {
	glPushMatrix();
		glScalef(12./9.,0.75,1.);
		for (int i=0 ; i<4 ; i++) {
			corridorBorderPos[i] += obstacleSpeed;
			if (corridorBorderPos[i] >= -1) {
				corridorBorderPos[i] -= OBSTACLE_LENGTH;
			}
			glColor3f(1.0+corridorBorderPos[i]/(1.*OBSTACLE_LENGTH) , 1.0+corridorBorderPos[i]/(1.*OBSTACLE_LENGTH) , 1.0+corridorBorderPos[i]/(1.*OBSTACLE_LENGTH));
			glPushMatrix();
				glTranslatef(0.0,0.0,corridorBorderPos[i]);
				drawUnfilledSquare();
			glPopMatrix();
		}
	glPopMatrix();
	//printf("{%f,%f,%f,%f}\n",corridorBorderPos[0],corridorBorderPos[1],corridorBorderPos[2],corridorBorderPos[3]);
}

void initObstacle() {
	srand(time( NULL ));
	for (int i=0 ; i<NB_OBSTACLES ; i++) {
		int wall = rand()%4;
		switch (wall) {
			case 0 : obstacles[i].wall = 'b';
				 break;
			case 1 : obstacles[i].wall = 't';
				 break;
			case 2 : obstacles[i].wall = 'l';
				 break;
			case 3 : obstacles[i].wall = 'r';
				 break;
		}
		
		obstacles[i].pos = -OBSTACLE_SPACE*(i+1);
		wall = rand()%2;
		switch (wall) {
			case 0 : obstacles[i].size = 1./3;
				 break;
			case 1 : obstacles[i].size = 2./3;
				 break;
		}
		//obstacles[i].size = 1./3;
	}
}

/*void moveObstacles() {
	for (int i=nbObstacles-1 ; i>=0 ; i--) {
		obstacles[i].pos += obstacleSpeed;
	}
}*/

void drawObstacles(GLuint textures) {
	glPushMatrix();
		glScalef(12./9.,0.75,1.);
		for (int i=0 ; i<NB_OBSTACLES ; i++) {
			obstacles[i].pos += obstacleSpeed;
			if (obstacles[i].pos >= -OBSTACLE_LENGTH && obstacles[i].pos <= 0) {
				glColor3f(1.0+obstacles[i].pos/(1.*OBSTACLE_LENGTH) , 1.0+obstacles[i].pos/(1.*OBSTACLE_LENGTH) , 1.0+obstacles[i].pos/(1.*OBSTACLE_LENGTH));
				glPushMatrix();
					switch (obstacles[i].wall) {
						case 'b' : glScalef(1.0,obstacles[i].size,1.0);
							   //glTranslatef(0.0,-1.0,obstacles[i].pos);
							   glTranslatef(0.0,-1./pow(3*obstacles[i].size,2),obstacles[i].pos);
			 				   break;
						case 't' : glScalef(1.0,obstacles[i].size,1.0);
							   //glTranslatef(0.0,1.0,obstacles[i].pos);
							   glTranslatef(0.0,1./pow(3*obstacles[i].size,2),obstacles[i].pos);
				 			   break;
						case 'l' : glScalef(obstacles[i].size,1.0,1.0);
							   //glTranslatef(-1.0,0.0,obstacles[i].pos);
							   glTranslatef(-1./pow(3*obstacles[i].size,2),0.0,obstacles[i].pos);
			 				   break;
						case 'r' : glScalef(obstacles[i].size,1.0,1.0);
							   //glTranslatef(1.0,0.0,obstacles[i].pos);
							   glTranslatef(1./pow(3*obstacles[i].size,2),0.0,obstacles[i].pos);
				 			   break;
					}
					drawSquareTexture(textures);
				glPopMatrix();
			}
		}
		if (obstacles[NB_OBSTACLES-1].pos >= -3*OBSTACLE_SPACE) { // Mur de fin de niveau-jeu
			glColor3f(1.0+(obstacles[NB_OBSTACLES-1].pos-OBSTACLE_SPACE)/(1.*OBSTACLE_LENGTH) , 0. , 0.);
			glTranslatef(0.0,0.0,obstacles[NB_OBSTACLES-1].pos-OBSTACLE_SPACE);
			drawSquare();
		}
	glPopMatrix();
}
