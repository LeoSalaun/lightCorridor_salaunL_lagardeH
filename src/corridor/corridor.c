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
static const char WINDOW_TITLE[] = "base";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Virtual windows space */
// Space is defined in interval -1 and 1 on x and y axes
static const float GL_VIEW_SIZE = 20.;

/* Error handling function */
void onError(int error, const char *description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

void onWindowResized(GLFWwindow *window, int width, int height)
{
	aspectRatio = width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspectRatio, Z_NEAR, Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_A:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_L:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GLFW_KEY_P:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		/*case GLFW_KEY_R :
			flag_animate_rot_arm = 1-flag_animate_rot_arm;
			break;
		case GLFW_KEY_T :
			flag_animate_rot_scale = 1-flag_animate_rot_scale;
			break;*/
		case GLFW_KEY_KP_9:
			if (dist_zoom < 100.0f)
				dist_zoom *= 1.1;
			printf("Zoom is %f\n", dist_zoom);
			break;
		case GLFW_KEY_KP_3:
			if (dist_zoom > 1.0f)
				dist_zoom *= 0.9;
			printf("Zoom is %f\n", dist_zoom);
			break;
		case GLFW_KEY_UP:
			if (phy > 2)
				phy -= 2;
			printf("Phy %f\n", phy);
			break;
		case GLFW_KEY_DOWN:
			if (phy <= 88.)
				phy += 2;
			printf("Phy %f\n", phy);
			break;
		case GLFW_KEY_LEFT:
			theta -= 5;
			break;
		case GLFW_KEY_RIGHT:
			theta += 5;
			break;
		default:
			fprintf(stdout, "Touche non gérée (%d)\n", key);
		}
	}
}

int main(int argc, char **argv)
{
	/* GLFW initialisation */
	GLFWwindow *window;
	if (!glfwInit())
		return -1;

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

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetKeyCallback(window, onKey);

	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	glPointSize(4.0);

	/* Load images */
	int width, height, nb_canaux;

	unsigned char *plafondcorridor = stbi_load("src/corridor/plafondcorridor.png", &width, &height, &nb_canaux, 0);
	unsigned char *cotecorridor = stbi_load("src/corridor/cotecorridor.png", &width, &height, &nb_canaux, 0);

	if (plafondcorridor == NULL)
	{
		printf("Erreur lors du chargement de l'image !\n");
	}
	else
	{
		printf("Image correctement chargée\n");
	}
	if (cotecorridor == NULL)
	{
		printf("Erreur lors du chargement de l'image !\n");
	}
	else
	{
		printf("Image correctement chargée\n");
	}
	GLuint textures;

	glGenTextures(1, &textures);

	glBindTexture(GL_TEXTURE_2D, textures);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, plafondcorridor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cotecorridor);

	glBindTexture(GL_TEXTURE_2D, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.5, 0.5, 0.5, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		/* RENDER HERE */

		// dessiner mur 9 gauche jaune
		glColor3f(1, 1, 0);
		glPushMatrix();
		glTranslatef(-8, 0., -GL_VIEW_SIZE);
		glScalef(4.5, 9, 85);
		glRotatef(90, 0, 1, 0);
		// texture
		glTexCoord2f(0, 1);
		// texture
		glTexCoord2f(1, 1);
		// texture
		glTexCoord2f(1, 0);
		// texture
		glTexCoord2f(0, 0);
		drawSquare();
		glPopMatrix();

		// dessiner mur 9 droit rouge
		glColor3f(1, 0, 0);
		glPushMatrix();
		glTranslatef(8, 0., -GL_VIEW_SIZE);
		glScalef(4.5, 9, 85);
		glRotatef(90, 0, 1, 0);
		// texture
		glTexCoord2f(0, 1);
		// texture
		glTexCoord2f(1, 1);
		// texture
		glTexCoord2f(1, 0);
		// texture
		glTexCoord2f(0, 0);
		drawSquare();
		glPopMatrix();

		// dessiner mur 16 haut bleu
		glColor3f(0, 0, 1);
		glPushMatrix();
		glTranslatef(0, 4.5, -GL_VIEW_SIZE);
		glScalef(16, 8, 85);
		glRotatef(90, 1, 0, 0);
		// texture
		glTexCoord2f(0, 1);
		// texture
		glTexCoord2f(1, 1);
		// texture
		glTexCoord2f(1, 0);
		// texture
		glTexCoord2f(0, 0);
		drawSquare();
		glPopMatrix();

		// dessiner mur 16 bas rose

		glPushMatrix();
		glTranslatef(0, -4.5, -GL_VIEW_SIZE);
		glScalef(16, 8, 85);
		glRotatef(90, 1, 0, 0);
		// texture
		glTexCoord2f(0, 1);
		// texture
		glTexCoord2f(1, 1);
		// texture
		glTexCoord2f(1, 0);
		// texture
		glTexCoord2f(0, 0);
		drawSquare();
		glPopMatrix();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if (elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
		}
	}

	stbi_image_free(plafondcorridor);
	stbi_image_free(cotecorridor);

	glDeleteTextures(1, &textures);

	glfwTerminate();
	return 0;
}
