#include <time.h>
#include "stb_image.h"
#include "3D_tools.h"

#include "obstacles.h"

double obstacleSpeed;
double corridorBorderPos[4] = {-10,-20,-30,-40};
Obstacle obstacles[nbObstacles];

void drawCorridorBorder() {
	glPushMatrix();
		glScalef(16.0,9.0,1.0);
		for (int i=0 ; i<4 ; i++) {
			corridorBorderPos[i] += obstacleSpeed;
			if (corridorBorderPos[i] == 0) {
				corridorBorderPos[i] = -40;
			}
			glColor3f(1.0+corridorBorderPos[i]/40 , 1.0+corridorBorderPos[i]/40 , 1.0+corridorBorderPos[i]/40);
			glPushMatrix();
				glTranslatef(0.0,0.0,corridorBorderPos[i]);
				drawUnfilledSquare();
			glPopMatrix();
		}
	glPopMatrix();
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
		
		obstacles[i].pos = -10*(i+1);
		obstacles[i].size = 1.0/3;
	}
}

/*void moveObstacles() {
	for (int i=nbObstacles-1 ; i>=0 ; i--) {
		obstacles[i].pos += obstacleSpeed;
	}
}*/

void drawObstacles(int first, int last) {
	if (first >= nbObstacles-1) {
		first = nbObstacles-1;
	}
	if (last <= 0) {
		last = 0;
	}
	
	glPushMatrix();
		glScalef(16.0,9.0,1.0);
		for (int i=first ; i>=last ; i--) {
			obstacles[i].pos += obstacleSpeed;
			if (obstacles[i].pos >= -40 && obstacles[i]. pos <= 0) {
				glColor3f(1.0+obstacles[i].pos/40 , 1.0+obstacles[i].pos/40 , 1.0+obstacles[i].pos/40);
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
	glPopMatrix();
}
