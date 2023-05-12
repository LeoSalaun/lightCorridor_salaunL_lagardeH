#include <time.h>
#include "stb_image.h"
#include "3D_tools.h"

#include "obstacles.h"

double obstacleSpeed;
double corridorBorderPos[4] = {-obstacleSpace,-2*obstacleSpace,-3*obstacleSpace,-4*obstacleSpace};
Obstacle obstacles[nbObstacles];

void drawCorridorBorder() {
	glPushMatrix();
		glScalef(16./9.,1.,1.);
		for (int i=0 ; i<4 ; i++) {
			corridorBorderPos[i] += obstacleSpeed;
			if (corridorBorderPos[i] >= -1) {
				corridorBorderPos[i] -= obstacleLength;
			}
			glColor3f(1.0+corridorBorderPos[i]/(1.*obstacleLength) , 1.0+corridorBorderPos[i]/(1.*obstacleLength) , 1.0+corridorBorderPos[i]/(1.*obstacleLength));
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
	
	for (int i=0 ; i<nbObstacles ; i++) {
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
		
		obstacles[i].pos = -obstacleSpace*(i+1);
		obstacles[i].size = 1./3;
	}
}

/*void moveObstacles() {
	for (int i=nbObstacles-1 ; i>=0 ; i--) {
		obstacles[i].pos += obstacleSpeed;
	}
}*/

void drawObstacles() {
	glPushMatrix();
		glScalef(16./9.,1.,1.);
		for (int i=0 ; i<nbObstacles ; i++) {
			obstacles[i].pos += obstacleSpeed;
			if (obstacles[i].pos >= -obstacleLength && obstacles[i].pos <= 0) {
				glColor3f(1.0+obstacles[i].pos/(1.*obstacleLength) , 1.0+obstacles[i].pos/(1.*obstacleLength) , 1.0+obstacles[i].pos/(1.*obstacleLength));
				glPushMatrix();
					switch (obstacles[i].wall) {
						case 'b' : glScalef(1.0,obstacles[i].size,1.0);
							   glTranslatef(0.0,-1.0,obstacles[i].pos);
							   drawSquare();
			 				   break;
						case 't' : glScalef(1.0,obstacles[i].size,1.0);
							   glTranslatef(0.0,1.0,obstacles[i].pos);
							   drawSquare();
				 			   break;
						case 'l' : glScalef(obstacles[i].size,1.0,1.0);
							   glTranslatef(-1.0,0.0,obstacles[i].pos);
							   drawSquare();
			 				   break;
						case 'r' : glScalef(obstacles[i].size,1.0,1.0);
							   glTranslatef(1.0,0.0,obstacles[i].pos);
							   drawSquare();
				 			   break;
					}
				glPopMatrix();
			}
		}
		if (obstacles[nbObstacles-1].pos >= -3*obstacleSpace) { // Mur de fin de niveau-jeu
			glColor3f(1.0+(obstacles[nbObstacles-1].pos-obstacleSpace)/(1.*obstacleLength) , 0. , 0.);
			glTranslatef(0.0,0.0,obstacles[nbObstacles-1].pos-obstacleSpace);
			drawSquare();
		}
	glPopMatrix();
}
