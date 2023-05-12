#include "draw_scene.h"
#include "3D_tools.h"

void drawBase()
{
	glPushMatrix();
	glScalef(3.0, 3.0, 1.0);
	drawCircle();
	glPopMatrix();
	glPushMatrix();
	glScalef(2.0, 2.0, 10.0);
	drawCone();
	glPopMatrix();
}

void drawArm()
{
	glPushMatrix();
	glScalef(2.4, 2.4, 2.4);
	drawSphere();
	glPopMatrix();

	glPushMatrix();
	glScalef(2.0, 10.0, 2.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCone();
	glRotatef(180, 1.0, 0.0, 0.0);
	drawCone();
	glPopMatrix();
}

void drawPan()
{
	glPushMatrix();
	glColor3f(0.38, 0.46, 0);
	glScalef(6.0, 6.0, 1.0);
	drawCircle();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	for (int i = 0; i < 3; i++)
	{
		glRotatef(120, 0, 0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(0, 0, 5.0);
		glVertex3f(0, 6.0, 0);
		glEnd();
	}
	glPopMatrix();
}

void drawFrame()
{
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5, 0, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.5, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.5);
	glEnd();
	glPopMatrix();
}

/*void drawSphere() {
	for (int i=0 ; i<360 ; i++) {
		glRotatef(1,0.,0.,1.);
		drawCircle();
	}
}*/
