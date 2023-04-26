#include "collisions.h"

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

/* Project variables */

extern Ball balle;
extern double xpos, ypos;

extern double obstacleSpeed;
//extern double corridorBorderPos[4] = {-10,-20,-30,-40};
extern Obstacle obstacles[nbObstacles];

int forward;

/* Error handling function */
void onError(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

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
			case GLFW_KEY_B :
				balle.sticky = 1;
				balle.speeX = 0;
				balle.speeY = 0;
				balle.speeZ = 0.25;
				break;
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !(balle.sticky) && forward) {
    	obstacleSpeed = obstacleSpace/20;
    }
    else if ((button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) || balle.sticky || !(forward)) {
    	obstacleSpeed = 0;
    }
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    	balle.sticky = 0;
	balle.speeZ = -0.25;
    }
}

void collCorridor() {
	if (balle.posX-1 <= 0 || balle.posX+1 >= 1280) {
		balle.speeX = -balle.speeX;
		balle.posX = round(balle.posX/1280)*1280;
	}
	if (balle.posY-1 <= 0 || balle.posY+1 >= 720) {
		balle.speeY = -balle.speeY;
		balle.posY = round(balle.posY/720)*720;
	}
}

void collWall() {
	forward = 1;
	for (int i=0 ; i<nbObstacles ; i++) {
		int x1 = 0, x2 = 1080, y1 = 0, y2 = 720;
		switch (obstacles[i].wall) {
			case 'b' : y1 = 480;
				   y2 = 720;
				   break;
			case 't' : y2 = 240;
				   break;
			case 'l' : x2 = 360;
				   break;
			case 'r' : x1 = 720;
				   x2 = 1080;
				   break;
		}
		if (fabs(obstacles[i].pos - balle.posZ) <= 1) {
			
			if ((fabs(obstacles[i].pos - balle.posZ) <= 1
				&& balle.posX >= x1
				&& balle.posX <= x2
				&& balle.posY >= y1
				&& balle.posY <= y2)
			    || sqrt(pow(x1-balle.posX,2) + pow(obstacles[i].pos-balle.posZ,2)) <= 1
			    || sqrt(pow(x2-balle.posX,2) + pow(obstacles[i].pos-balle.posZ,2)) <= 1
			    || sqrt(pow(y1-balle.posY,2) + pow(obstacles[i].pos-balle.posZ,2)) <= 1
			    || sqrt(pow(y2-balle.posY,2) + pow(obstacles[i].pos-balle.posZ,2)) <= 1) {
				balle.speeZ = -balle.speeZ;
				balle.posZ = round(balle.posZ-obstacles[i].pos) + obstacles[i].pos;
			}
		}
		
		if (obstacles[i].pos == -1.5 && (fabs(xpos - x1) <= 148 || fabs(xpos - x2) <= 148
					      || fabs(ypos - y1) <= 148 || fabs(ypos - y2) <= 148)) {
			forward = 0;
		}
	}
}

void collRaquette() {
	if (!(balle.sticky) && balle.posZ >= -2. && fabs(xpos - balle.posX) <= 148 && fabs(ypos - balle.posY) <= 148) {
		balle.speeZ = -balle.speeZ;
		balle.speeX = -(xpos - balle.posX);
		balle.speeY = -(ypos - balle.posY);
		balle.posZ = -2.;
	}
	else if (!(balle.sticky) && balle.posZ >= -2.) {
		balle.sticky = 1;
		balle.speeX = 0;
		balle.speeY = 0;
		balle.speeZ = 0.25;
	}
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
	glEnable(GL_DEPTH_TEST);
	
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
	
	initObstacle();
	
	balle.sticky = 1;
	balle.speeX = 0;
	balle.speeY = 0;
	balle.speeZ = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		/* RENDER HERE */
		
		glPushMatrix();
			glScalef(4./GL_VIEW_SIZE,4./GL_VIEW_SIZE,2./GL_VIEW_SIZE);
			// On déplace de GL_VIEW_SIZE en avant pour avoir les éléments devant la caméra
			
			drawCorridorBorder();
				
			drawObstacles();
			
			glfwGetCursorPos(window, &xpos, &ypos); // On reçoit la position du curseur de la souris
			
			moveBall();
			
			if (!(balle.sticky)) {
				collCorridor();
				
				collWall();
				
				collRaquette();
			}
			
			drawRaquette();
			
			drawBall();
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
