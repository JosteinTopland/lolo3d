#include "globals.h"

SDL_Window* window;
const int windowWidth = 800;
const int windowHeight = 600;

int run = 1;
GLfloat cameraX = 0;
GLfloat cameraY = -3.0f;

GLuint modelMatId;
GLuint viewMatId;
GLuint projMatId;
GLuint enableTexture;

mat4 modelMat;
mat4 viewMat;
mat4 projMat;