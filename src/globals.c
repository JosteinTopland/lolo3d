#include "globals.h"

SDL_Window* window;
const int windowWidth = 1024;
const int windowHeight = 768;

int run = 1;
GLfloat cameraX = 0;
GLfloat cameraY = -3.0f;

GLint modelMatId;
GLint viewMatId;
GLint projMatId;
GLint enableTexture;
GLint diffuseColor;

mat4 modelMat;
mat4 viewMat;
mat4 projMat;
