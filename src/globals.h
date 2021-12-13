#ifndef DATA_H
#define DATA_H

#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <GL/glew.h>

SDL_Window* window;
const int windowWidth;
const int windowHeight;

int run;
GLfloat cameraX;
GLfloat cameraY;

GLint modelMatId;
GLint viewMatId;
GLint projMatId;
GLint enableTexture;
GLint diffuseColor;

extern mat4 modelMat;
extern mat4 viewMat;
extern mat4 projMat;

#endif