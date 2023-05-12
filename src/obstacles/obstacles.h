typedef struct Obstacle {
	double pos;
	double size;
	char wall;
} Obstacle;

#define nbObstacles 10
#define obstacleSpace 3.
#define obstacleLength 4.*obstacleSpace

void drawUnfilledSquare();

void drawCorridorBorder();

void initObstacle();

void drawObstacles();
