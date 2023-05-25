#include "bonus/3D_tools.h"

/* Camera parameters and functions */
float theta = 45.0f;	 // Angle between x axis and viewpoint
float phy = 60.0f;		 // Angle between z axis and viewpoint
float dist_zoom = 30.0f; // Distance between origin and viewpoint

void setCamera()
{
	gluLookAt(dist_zoom * cos(toRad(theta)) * sin(toRad(phy)),
			  dist_zoom * sin(toRad(theta)) * sin(toRad(phy)),
			  dist_zoom * cos(toRad(phy)),
			  0.0, 0.0, 0.0,
			  0.0, 0.0, 1.0);
}

/* Convert degree to radians */
float toRad(float deg)
{
	return deg * M_PI / 180.0f;
}

void drawSquare()
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
}

void drawCircle()
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 0.0);
	float step_rad = 2 * M_PI / (float)NB_SEG_CIRCLE;
	for (int i = 0; i <= NB_SEG_CIRCLE; i++)
	{
		glVertex3f(cos(i * step_rad), sin(i * step_rad), 0.0f);
	}
	glEnd();
}

void drawUnfilledCircle()
{
	glBegin(GL_LINES);
	float step_rad = 2 * M_PI / (float)NB_SEG_CIRCLE;
	for (int i = 0; i <= NB_SEG_CIRCLE; i++)
	{
		glVertex3f(cos(i * step_rad), sin(i * step_rad), 0.0f);
		glVertex3f(cos((i+1) % NB_SEG_CIRCLE * step_rad), sin((i+1) % NB_SEG_CIRCLE * step_rad), 0.0f);
	}
	glEnd();
}

void drawCylinder()
{
	/*glBegin(GL_QUAD_STRIP);		PROPOSITION POUR DESSINER UN CYLINDRE EN 3D
	glVertex3f(0.0, 0.0, 0.0);
	float step_rad = 2 * M_PI / (float)NB_SEG_CYLINDER;
	for (int i = 0; i <= NB_SEG_CYLINDER; i++)
	{
		glVertex3f(cos(i * step_rad), 0.0f, sin(i * step_rad));
		glVertex3f(cos(i * step_rad), 1.0f, sin(i * step_rad));
	}
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	
	glBegin(GL_POLYGON);
	for (int i = 0; i <= NB_SEG_CYLINDER; i++)
	{
		glVertex3f(cos(i * step_rad), 1.0f, sin(i * step_rad));
	}
	glEnd();*/
	gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, NB_SEG_CIRCLE, NB_SEG_CIRCLE);
}

void drawCone()
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 1.0);
	float step_rad = 2 * M_PI / (float)NB_SEG_CIRCLE;
	for (int i = 0; i <= NB_SEG_CIRCLE; i++)
	{
		glVertex3f(cos(i * step_rad), sin(i * step_rad), 0.0f);
	}
	glEnd();
}

void drawSphere()
{
	gluSphere(gluNewQuadric(), 1.0, NB_SEG_CIRCLE, NB_SEG_CIRCLE);
}
