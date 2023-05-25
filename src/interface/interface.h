typedef struct Player {
	int nbVies;
	int forward;
	int sticky;
	int menu;
} Player;

void drawVies(GLuint textures);

void drawSticky(GLuint textures);

void drawInterface(GLuint texturesVie, GLuint texturesSticky);
