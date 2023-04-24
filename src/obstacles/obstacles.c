#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "3D_tools.h"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1280;
static const unsigned int WINDOW_HEIGHT = 720;
static const char WINDOW_TITLE[] = "TD04 Ex01";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Virtual windows space */
// Space is defined in interval -1 and 1 on x and y axes
static const float GL_VIEW_SIZE = 20.;

/* Error handling function */
void onError(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

/* Project variable definition */

struct Obstacle {
	int pos;
	int size;
	char wall;
}Obstacle;

static double obstacleSpeed = 0.5;
static double corridorBorderPos[4] = {-10,-20,-30,-40};

static int nbObstacles = 10;
static Obstacle obstacles[nbObstacles];

void onWindowResized(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,aspectRatio,Z_NEAR,Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_A :
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_L :
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case GLFW_KEY_P :
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case GLFW_KEY_KP_9 :
				if(dist_zoom<100.0f) dist_zoom*=1.1;
				printf("Zoom is %f\n",dist_zoom);
				break;
			case GLFW_KEY_KP_3 :
				if(dist_zoom>1.0f) dist_zoom*=0.9;
				printf("Zoom is %f\n",dist_zoom);
				break;
			case GLFW_KEY_UP :
				if (phy>2) phy -= 2;
				printf("Phy %f\n",phy);
				break;
			case GLFW_KEY_DOWN :
				if (phy<=88.) phy += 2;
				printf("Phy %f\n",phy);
				break;
			case GLFW_KEY_LEFT :
				theta -= 5;
				break;
			case GLFW_KEY_RIGHT :
				theta += 5;
				break;
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    	
    }
}

void drawUnfilledSquare() { // On dessine un carré vide en dessinant 4 lignes
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
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
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

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
int main(int argc, char** argv) 
{
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	glPointSize(4.0);
	
	/* Load images */
	/*int width, height, nb_canaux;
	
	unsigned char* image = stbi_load("doc/IMAC.png",&width,&height,&nb_canaux,0);
	
	if (image == NULL) {
		printf("Erreur lors du chargement de l'image !\n");
	}
	else {
		printf("Image correctement chargée\n");
	}
	
	GLuint textures;
	
	glGenTextures(1, &textures);
	
	glBindTexture(GL_TEXTURE_2D, textures);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	
	glBindTexture(GL_TEXTURE_2D, 0);*/
	
	
	/* Initialize variables */
	
	for (int i=0 ; i<nbObstacles ; i++) {
		srand();
		int wall = rand()%4;
		switch (wall) {
			case 0 : strcpy(obstacles[i].wall,'b');
			case 1 : strcpy(obstacles[i].wall,'t');
			case 2 : strcpy(obstacles[i].wall,'l');
			default : strcpy(obstacles[i].wall,'r');
		}
		
		obstacles[i].pos = -10*(i+1);
		obstacles[i].size = 1/3;
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		/* RENDER HERE */
		
		drawCorridorBorder();
		
		glPushMatrix();
			glScalef(16.0,9.0,1.0);
			for (int i=0 ; i<nbObstacles ; i++) {
				glPushMatrix();
					switch (nbObstacles[i].wall) {
						case 'b' : glScalef(1.0,nbObstacles[i].size,1.0);
							   glTranslatef(0.0,-1.0,nbObstacles[i].pos);
						case 't' : glScalef(1.0,nbObstacles[i].size,1.0);
							   glTranslatef(0.0,1.0,nbObstacles[i].pos);
						case 'l' : glScalef(nbObstacles[i].size,1.0,1.0);
							   glTranslatef(-1.0,0.0,nbObstacles[i].pos);
						case 'r' : glScalef(nbObstacles[i].size,1.0,1.0);
							   glTranslatef(1.0,0.0,nbObstacles[i].pos);
					}
				glPopMatrix();
			}
		glPopMatrix();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS) 
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}
	}
	
	/*stbi_image_free(image);
	
	glDeleteTextures(1, &textures);*/

	glfwTerminate();
	return 0;
}
