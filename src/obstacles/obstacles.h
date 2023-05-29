typedef struct Obstacle
{
	double pos;
	double size;
	char wall;
} Obstacle;

#define NB_OBSTACLES 10
#define OBSTACLE_SPACE 3.
#define OBSTACLE_LENGTH 4. * OBSTACLE_SPACE

void drawUnfilledSquare();

void drawCorridorBorder();

void initObstacle();

void drawObstacles(GLuint texturesAraignee, GLuint texturesCreeper, GLuint texturesSquelette, GLuint texturesZombie, GLuint texturesFin);
