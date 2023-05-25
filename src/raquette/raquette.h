typedef struct Ball {
	double posX;
	double posY;
	double posZ;
	
	double speeX;
	double speeY;
	double speeZ;
	
	int sticky;
} Ball;

void drawUnfilledSquare();

void drawRaquette();

void moveBall();

void drawBall(GLuint textures);
