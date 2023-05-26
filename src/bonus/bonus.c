#include <time.h>
#include "stb_image.h"
#include "3D_tools.h"

#include "bonus.h"

int rotateAngle;

Bonus bonus[NB_BONUS];

extern double obstacleSpeed;

void drawBonusLife() {
	glPushMatrix();
        	glColor3f(0.,1.,0.);
        	glRotatef(rotateAngle,0.,1.,0.);
        	glPushMatrix();
        		glScalef(4./5,4./5,2./5);
        		glTranslatef(0.,-1./10,0.);
        		drawSphere();
        	glPopMatrix();
        	glPushMatrix();
        		glScalef(1./5,1.,1./5);
        		glRotatef(-90,1.,0.,0.);
        		drawCylinder();
        	glPopMatrix();
        	glPushMatrix();
        		glScalef(3./10,1./5,3./10);
        		glTranslatef(0.,4.,0.);
        		glRotatef(-90,1.,0.,0.);
        		drawCylinder();
        	glPopMatrix();
        	glPushMatrix();
        		glScalef(3./10,1./5,3./10);
        		glTranslatef(0.,4.,0.);
        		glRotatef(-90,1.,0.,0.);
        		drawCircle();
        	glPopMatrix();
        	glColor3f(0.5,0.2,0.);
        	glPushMatrix();
        		glScalef(1./5,1./5,1./5);
        		glTranslatef(0.,5.,0.);
        		glRotatef(-90,1.,0.,0.);
        		drawCylinder();
        	glPopMatrix();
        	glPushMatrix();
        		glScalef(1./5,1./5,1./5);
        		glTranslatef(0.,5.,0.);
        		glRotatef(-90,1.,0.,0.);
        		drawCircle();
        	glPopMatrix();
        glPopMatrix();
}

void drawBonusStickyFace() {
	glPushMatrix();
        	glTranslatef(-1.,-1.,0.);
       		glScalef(2.,2.,1.);
        	glBegin(GL_LINES);
        		glVertex3f(0., 1., 0.);
			glVertex3f(1., 0., 0.);
			
			glVertex3f(0., 0., 0.);
			glVertex3f(1., 1., 0.);
			
			glVertex3f(0., 0.5, 0.);
			glVertex3f(1., 0.5, 0.);
        	glEnd();
        glPopMatrix();
        glScalef(3./4,3./4,1.);
        drawUnfilledCircle();
        glScalef(2./4,2./4,1.);
        drawUnfilledCircle();
        glScalef(1./4,1./4,1.);
        drawUnfilledCircle();
}

void drawBonusSticky() {
	glPushMatrix();
        	//glTranslatef(0.,0.,-GL_VIEW_SIZE/3);
        	glColor3f(1.,1.,1.);
        	glRotatef(rotateAngle,0.,1.,0.);
        	glPushMatrix();
        		glRotatef(45,0.,1.,0.);
        		drawBonusStickyFace();
        	glPopMatrix();
        	glPushMatrix();
        		glRotatef(135,0.,1.,0.);
        		drawBonusStickyFace();
        	glPopMatrix();
        	glTranslatef(-1.,-1.,0.);
        	glScalef(2.,2.,1.);
        	glBegin(GL_LINES);
        		glVertex3f(0.5, 1., 0.);
			glVertex3f(0.5, 0., 0.);
        	glEnd();
        glPopMatrix();
}

void initBonus() {
	srand(time( NULL ));
	for (int i=0 ; i<NB_BONUS ; i++) {
		bonus[i].type = rand()%2;
		bonus[i].posX = rand()%720+280;
		bonus[i].posY = 720-(rand()%380);
		bonus[i].posZ = -(rand()%2+i*6+7);
		bonus[i].visible = 1;
	}
}

void moveBonus() {
	for (int i=0 ; i<NB_BONUS ; i++) {
		bonus[i].posZ += obstacleSpeed;
	}
}

void drawBonus() {
	glPushMatrix();
		glScalef(1./1280,1./1280,1.);
		for (int i=0 ; i<NB_BONUS ; i++) {
			if (bonus[i].visible) {
				glPushMatrix();
					//glTranslatef(*3/4-12,*3/4-6.75,bonus[i].posZ);
					glTranslatef(bonus[i].posX-640,bonus[i].posY-640,bonus[i].posZ);
					glScalef(128.,128.,1.);
					switch (bonus[i].type) {
						case 0 : drawBonusLife();
							 break;
						case 1 : drawBonusSticky();
							 break;
					}
				glPopMatrix();
			}
		}
	glPopMatrix();
}
