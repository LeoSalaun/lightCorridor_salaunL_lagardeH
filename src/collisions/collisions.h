#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "3D_tools.h"

#include "../corridor/corridor.h"
#include "../raquette/raquette.h"
#include "../obstacles/obstacles.h"

void collCorridor();

void collWall();

void collRaquette();
