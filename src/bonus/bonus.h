typedef struct Bonus {
	int type;
	double posX;
	double posY;
	double posZ;
	int visible;
} Bonus;

#define ROTATE_SPEED 3
#define NB_BONUS 4

void drawBonusLife();

void drawBonusStickyFace();

void drawBonusSticky();

void initBonus();

void drawBonus();
