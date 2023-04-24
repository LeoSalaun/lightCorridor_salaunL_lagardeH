typedef struct Obstacle {
	double pos;
	double size;
	char wall;
} Obstacle;

#define nbObstacles 10

void drawUnfilledSquare();

void drawCorridorBorder();

void initObstacle();

void drawObstacles(int first, int last);
