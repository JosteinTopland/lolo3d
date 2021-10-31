#ifndef DATA_H
#define DATA_H

#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <GL/glew.h>

extern SDL_Window* window;
extern const int windowWidth;
extern const int windowHeight;

extern int run;
extern GLfloat cameraX;
extern GLfloat cameraY;

extern GLuint modelMatId;
extern GLuint viewMatId;
extern GLuint projMatId;
extern GLuint enableTexture;

extern mat4 modelMat;
extern mat4 viewMat;
extern mat4 projMat;

#endif