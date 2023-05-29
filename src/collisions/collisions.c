#include "collisions.h"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1280;
static const unsigned int WINDOW_HEIGHT = 720;
static const char WINDOW_TITLE[] = "TD04 Ex01";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 120.;

/* Virtual windows space */
// Space is defined in interval -1 and 1 on x and y axes

/* Project variables */

extern Ball balle;
extern double xpos, ypos;

extern double obstacleSpeed;
// extern double corridorBorderPos[4] = {-10,-20,-30,-40};
extern Obstacle obstacles[NB_OBSTACLES];

extern int rotateAngle;
extern Player player;

extern Bonus bonus[NB_BONUS];

int isDown;

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
		case GLFW_KEY_B:
			balle.sticky = 1;
			balle.speeX = 0;
			balle.speeY = 0;
			balle.speeZ = -0.05;
			break;
		default:
			fprintf(stdout, "Touche non gérée (%d)\n", key);
		}
	}
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (!(player.menu))
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			isDown = 1;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			isDown = 0;
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			balle.sticky = 0;
			balle.speeZ = -0.05;
		}
	}
}

void handleRightMouseButton()
{
	if (isDown && !(balle.sticky) && player.forward)
	{
		obstacleSpeed = OBSTACLE_SPACE / 80;
	}
	else if (!(isDown) || balle.sticky || !(player.forward))
	{
		obstacleSpeed = 0;
	}
}

void collCorridor()
{
	if (balle.posX - 1 <= 0 || balle.posX + 1 >= 1280)
	{
		balle.speeX = -balle.speeX;
		// balle.posX = round(balle.posX/1280)*1280;
	}
	if (balle.posY - 1 <= 0 || balle.posY + 1 >= 720)
	{
		balle.speeY = -balle.speeY;
		// balle.posY = round(balle.posY/720)*720;
	}
}

void collWall()
{
	for (int i = 0; i < NB_OBSTACLES; i++)
	{
		int x1 = 0, x2 = 1280, y1 = 0, y2 = 720;
		switch (obstacles[i].wall)
		{
		case 'b':
			y1 = 720 * (1 - obstacles[i].size);
			break;
		case 't':
			y2 = 720 * obstacles[i].size;
			break;
		case 'l':
			x2 = 1280 * obstacles[i].size;
			break;
		case 'r':
			x1 = 1280 * (1 - obstacles[i].size);
			break;
		}
		if (fabs(obstacles[i].pos - balle.posZ) <= 1)
		{

			if ((fabs(obstacles[i].pos - balle.posZ) <= 0.5 && balle.posX >= x1 && balle.posX <= x2 && balle.posY >= y1 && balle.posY <= y2) || sqrt(pow(x1 - balle.posX, 2) + pow(obstacles[i].pos - balle.posZ, 2)) <= 1 || sqrt(pow(x2 - balle.posX, 2) + pow(obstacles[i].pos - balle.posZ, 2)) <= 1 || sqrt(pow(y1 - balle.posY, 2) + pow(obstacles[i].pos - balle.posZ, 2)) <= 1 || sqrt(pow(y2 - balle.posY, 2) + pow(obstacles[i].pos - balle.posZ, 2)) <= 1)
			{
				balle.speeZ = -balle.speeZ;
				// balle.posZ = round(balle.posZ-obstacles[i].pos) + obstacles[i].pos;
			}
		}

		// printf("%f %d %d - %f %d %d - %f\n",xpos,x1,x2,(ypos-720),y1,y2,obstacles[i].pos);
		if (obstacles[i].pos >= -0.9 && obstacles[i].pos <= -0.5 && xpos >= x1 && xpos <= x2 && ypos >= y1 && ypos <= y2)
		{
			player.forward = 0;
		}
	}
	printf("%f - %f - %f\n", xpos, (ypos - 720), obstacles[0].pos);
}

void collRaquette(GLFWwindow *window)
{
	if (!(balle.sticky) && balle.posZ >= -0.5 && fabs(xpos - balle.posX) <= 148 && fabs(ypos - balle.posY) <= 148 && !(player.sticky))
	{
		balle.speeZ = -balle.speeZ;
		balle.speeX = (xpos - balle.posX) * balle.speeZ;
		balle.speeY = (ypos - balle.posY) * balle.speeZ;
		balle.posZ = -0.6;
	}
	else if (!(balle.sticky) && balle.posZ >= -0.5 && fabs(xpos - balle.posX) <= 148 && fabs(ypos - balle.posY) <= 148 && player.sticky)
	{
		balle.sticky = 1;
		balle.speeX = 0;
		balle.speeY = 0;
		balle.speeZ = 0.25;
		player.sticky = 0;
	}
	else if (!(balle.sticky) && balle.posZ >= -0.5)
	{
		balle.sticky = 1;
		balle.speeX = 0;
		balle.speeY = 0;
		balle.speeZ = 0.25;
		player.nbVies--;
		if (player.nbVies == 0)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	if (obstacles[NB_OBSTACLES - 1].pos - OBSTACLE_SPACE >= -0.5)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void collBonus()
{
	for (int i = 0; i < NB_BONUS; i++)
	{
		printf("%d -> %f %f %f\n", i, bonus[i].posX, bonus[i].posY, bonus[i].posZ);
		printf("%f %f\n", xpos, ypos);
		if (bonus[i].visible && bonus[i].posZ <= 1. && bonus[i].posZ >= -1.5 && sqrt(pow(xpos - bonus[i].posX, 2) + pow(ypos - bonus[i].posY, 2)) <= 100 && (!(bonus[i].type) || (!(player.sticky) && bonus[i].type)))
		{
			bonus[i].visible = 0;
			switch (bonus[i].type)
			{
			case 0:
				player.nbVies++;
				break;
			case 1:
				player.sticky = 1;
				break;
			}
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
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	glPointSize(4.0);
	glEnable(GL_DEPTH_TEST);

	/* Load images */

	int widthTop, heightTop, nb_canauxTop, widthBottom, heightBottom, nb_canauxBottom, widthSides, heightSides, nb_canauxSides, widthVie, heightVie, nb_canauxVie, widthSticky, heightSticky, nb_canauxSticky, widthBall, heightBall, nb_canauxBall, widthFin, heightFin, nb_canauxFin, widthAraignee, heightAraignee, nb_canauxAraignee, widthMenu, heightMenu, nb_canauxMenu, widthCreeper, heightCreeper, nb_canauxCreeper, widthMort, heightMort, nb_canauxMort, widthSquelette, heightSquelette, nb_canauxSquelette, widthVictoire, heightVictoire, nb_canauxVictoire, widthZombie, heightZombie, nb_canauxZombie;

	//---------------------Top----------------------
	unsigned char *imageTop = stbi_load("doc/plafondcorridor.png", &widthTop, &heightTop, &nb_canauxTop, 0);

	if (imageTop == NULL)
	{
		printf("Erreur lors du chargement de l'image de plafond !\n");
	}
	else
	{
		printf("Image de plafond correctement chargée\n");
	}
	//------------------Bottom-------------------------
	unsigned char *imageBottom = stbi_load("doc/solcorridor.png", &widthBottom, &heightBottom, &nb_canauxBottom, 0);

	if (imageBottom == NULL)
	{
		printf("Erreur lors du chargement de l'image de sol !\n");
	}
	else
	{
		printf("Image de sol correctement chargée\n");
	}
	//------------------------Sides-------------------
	unsigned char *imageSides = stbi_load("doc/cotecorridor.png", &widthSides, &heightSides, &nb_canauxSides, 0);

	if (imageSides == NULL)
	{
		printf("Erreur lors du chargement de l'image de côtés !\n");
	}
	else
	{
		printf("Image de côtés correctement chargée\n");
	}
	//-------------------Vie------------------------
	unsigned char *imageVie = stbi_load("doc/vie.png", &widthVie, &heightVie, &nb_canauxVie, 0);

	if (imageVie == NULL)
	{
		printf("Erreur lors du chargement de l'image de vie !\n");
	}
	else
	{
		printf("Image de vie correctement chargée\n");
	}
	//-------------------------Sticky------------------
	unsigned char *imageSticky = stbi_load("doc/web.png", &widthSticky, &heightSticky, &nb_canauxSticky, 0);

	if (imageSticky == NULL)
	{
		printf("Erreur lors du chargement de l'image de toile !\n");
	}
	else
	{
		printf("Image de toile correctement chargée\n");
	}
	//----------------------Ball---------------------
	unsigned char *imageBall = stbi_load("doc/pearl.png", &widthBall, &heightBall, &nb_canauxBall, 0);

	if (imageBall == NULL)
	{
		printf("Erreur lors du chargement de l'image de balle !\n");
	}
	else
	{
		printf("Image de balle correctement chargée\n");
	}
	//---------------------------Fin----------------
	unsigned char *imageFin = stbi_load("doc/FIN.png", &widthFin, &heightFin, &nb_canauxFin, 0);

	if (imageFin == NULL)
	{
		printf("Erreur lors du chargement de l'image de Fin !\n");
	}
	else
	{
		printf("Image de Fin correctement chargée\n");
	}
	//--------------------Araignee-----------------------
	unsigned char *imageAraignee = stbi_load("doc/araignée.png", &widthAraignee, &heightAraignee, &nb_canauxAraignee, 0);

	if (imageAraignee == NULL)
	{
		printf("Erreur lors du chargement de l'image de Araignee !\n");
	}
	else
	{
		printf("Image de Araignee correctement chargée\n");
	}
	//-------------------Menu------------------------
	unsigned char *imageMenu = stbi_load("doc/menu.png", &widthMenu, &heightMenu, &nb_canauxMenu, 0);

	if (imageMenu == NULL)
	{
		printf("Erreur lors du chargement de l'image de Menu !\n");
	}
	else
	{
		printf("Image de Menu correctement chargée\n");
	}
	//-------------------------Creeper------------------
	unsigned char *imageCreeper = stbi_load("doc/creeper.png", &widthCreeper, &heightCreeper, &nb_canauxCreeper, 0);

	if (imageCreeper == NULL)
	{
		printf("Erreur lors du chargement de l'image de Creeper !\n");
	}
	else
	{
		printf("Image de Creeper correctement chargée\n");
	}
	//-------------------Mort------------------------
	unsigned char *imageMort = stbi_load("doc/mort.png", &widthMort, &heightCreeper, &nb_canauxMort, 0);

	if (imageMort == NULL)
	{
		printf("Erreur lors du chargement de l'image de mort !\n");
	}
	else
	{
		printf("Image de Mort correctement chargée\n");
	}
	//----------------------Squelette---------------------
	unsigned char *imageSquelette = stbi_load("doc/squeletteobstacle.png", &widthSquelette, &heightSquelette, &nb_canauxSquelette, 0);

	if (imageSquelette == NULL)
	{
		printf("Erreur lors du chargement de l'image de Squelette !\n");
	}
	else
	{
		printf("Image de Squelette correctement chargée\n");
	}
	//----------------------Victoire---------------------
	unsigned char *imageVictoire = stbi_load("doc/victoire.png", &widthVictoire, &heightVictoire, &nb_canauxVictoire, 0);

	if (imageVictoire == NULL)
	{
		printf("Erreur lors du chargement de l'image de Victoire !\n");
	}
	else
	{
		printf("Image de Victoire correctement chargée\n");
	}
	//----------------------Zombie---------------------
	unsigned char *imageZombie = stbi_load("doc/zombie.png", &widthZombie, &heightZombie, &nb_canauxZombie, 0);

	if (imageZombie == NULL)
	{
		printf("Erreur lors du chargement de l'image de Zombie !\n");
	}
	else
	{
		printf("Image de Zombie correctement chargée\n");
	}
	//-------------------------------------------
	GLuint texturesTop, texturesBottom, texturesSides, texturesVie, texturesSticky, texturesBall, texturesFin, texturesAraignee, texturesMenu, texturesCreeper, texturesMort, texturesSquelette, texturesVictoire, texturesZombie;
	//-----------------Top----------------------
	glGenTextures(1, &texturesTop);

	glBindTexture(GL_TEXTURE_2D, texturesTop);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTop, heightTop, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageTop);

	glBindTexture(GL_TEXTURE_2D, 0);
	//---------------Bottom---------------------
	glGenTextures(1, &texturesBottom);

	glBindTexture(GL_TEXTURE_2D, texturesBottom);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthBottom, heightBottom, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBottom);

	glBindTexture(GL_TEXTURE_2D, 0);
	//---------------Sides-------------------------
	glGenTextures(1, &texturesSides);

	glBindTexture(GL_TEXTURE_2D, texturesSides);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSides, heightSides, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSides);

	glBindTexture(GL_TEXTURE_2D, 0);
	//----------------Vie--------------------
	glGenTextures(1, &texturesVie);

	glBindTexture(GL_TEXTURE_2D, texturesVie);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthVie, heightVie, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageVie);

	glBindTexture(GL_TEXTURE_2D, 0);
	//---------------Sticky-----------------------
	glGenTextures(1, &texturesSticky);

	glBindTexture(GL_TEXTURE_2D, texturesSticky);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthSticky, heightSticky, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSticky);

	glBindTexture(GL_TEXTURE_2D, 0);
	//----------------Ball-------------------------//
	glGenTextures(1, &texturesBall);

	glBindTexture(GL_TEXTURE_2D, texturesBall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthBall, heightBall, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBall);

	glBindTexture(GL_TEXTURE_2D, 0);
	//-----------------Fin-----------------------//
	glGenTextures(1, &texturesFin);

	glBindTexture(GL_TEXTURE_2D, texturesFin);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthFin, heightFin, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageFin);

	glBindTexture(GL_TEXTURE_2D, 0);
	//---------------------araignée----------------------//
	glGenTextures(1, &texturesAraignee);

	glBindTexture(GL_TEXTURE_2D, texturesAraignee);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthAraignee, heightAraignee, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageAraignee);

	glBindTexture(GL_TEXTURE_2D, 0);
	//--------------------menu-----------------------//
	glGenTextures(1, &texturesMenu);

	glBindTexture(GL_TEXTURE_2D, texturesMenu);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthMenu, heightMenu, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageMenu);

	glBindTexture(GL_TEXTURE_2D, 0);
	//--------------------creeper-----------------------//
	glGenTextures(1, &texturesCreeper);

	glBindTexture(GL_TEXTURE_2D, texturesCreeper);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthCreeper, heightCreeper, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageCreeper);

	glBindTexture(GL_TEXTURE_2D, 0);
	//--------------------Mort-----------------------//
	glGenTextures(1, &texturesMort);

	glBindTexture(GL_TEXTURE_2D, texturesMort);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthMort, heightMort, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageMort);

	glBindTexture(GL_TEXTURE_2D, 0);
	//--------------------Squelette-----------------------//
	glGenTextures(1, &texturesSquelette);

	glBindTexture(GL_TEXTURE_2D, texturesSquelette);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthSquelette, heightSquelette, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSquelette);

	glBindTexture(GL_TEXTURE_2D, 0);
	//--------------------Victoire-----------------------//
	glGenTextures(1, &texturesVictoire);

	glBindTexture(GL_TEXTURE_2D, texturesVictoire);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthVictoire, heightVictoire, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageVictoire);

	glBindTexture(GL_TEXTURE_2D, 0);
	//--------------------Zombie-----------------------//
	glGenTextures(1, &texturesZombie);

	glBindTexture(GL_TEXTURE_2D, texturesZombie);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthZombie, heightZombie, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageZombie);

	glBindTexture(GL_TEXTURE_2D, 0);
	//-------------------------------------------
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Initialize variables */

	initObstacle();
	initBonus();

	balle.sticky = 1;
	balle.speeX = 0;
	balle.speeY = 0;
	balle.speeZ = 0;

	player.nbVies = 3;
	player.sticky = 0;
	player.menu = 0;

	rotateAngle = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		/* RENDER HERE */

		if (!(player.menu))
		{
			handleRightMouseButton();

			glfwGetCursorPos(window, &xpos, &ypos); // On reçoit la position du curseur de la souris

			player.forward = 1;

			glColor3f(1., 1., 1.);
			drawCorridor(texturesTop, texturesBottom, texturesSides);

			glPushMatrix();
			glScalef(6. / GL_VIEW_SIZE, 6. / GL_VIEW_SIZE, 2. / GL_VIEW_SIZE);
			glTranslatef(0., 0., -20.);
			glScalef(20., 20., 20.);

			drawCorridorBorder();

			drawObstacles(texturesAraignee, texturesCreeper, texturesSquelette, texturesZombie);

			moveBonus();

			drawBonus();

			moveBall();

			if (!(balle.sticky))
			{
				collCorridor();

				collWall();

				collRaquette(window);
			}

			collBonus();

			drawBall(texturesBall);
			glPopMatrix();

			drawRaquette();

			drawInterface(texturesVie, texturesSticky);

			rotateAngle += ROTATE_SPEED;
			if (rotateAngle == 360)
			{
				rotateAngle = 0;
			}
		}

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

	stbi_image_free(imageTop);

	glDeleteTextures(1, &texturesTop);

	stbi_image_free(imageBottom);

	glDeleteTextures(1, &texturesBottom);

	stbi_image_free(imageSides);

	glDeleteTextures(1, &texturesSides);

	stbi_image_free(imageVie);

	glDeleteTextures(1, &texturesVie);

	stbi_image_free(imageSticky);

	glDeleteTextures(1, &texturesSticky);

	stbi_image_free(imageBall);

	glDeleteTextures(1, &texturesBall);

	glDisable(GL_BLEND);

	glfwTerminate();
	return 0;
}
