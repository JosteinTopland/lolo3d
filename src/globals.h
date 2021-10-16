#ifndef DATA_H
#define DATA_H

#include <cglm/cglm.h>
#include <GL/glew.h>

extern const int windowWidth;
extern const int windowHeight;

extern GLuint modelMatId;
extern GLuint viewMatId;
extern GLuint projMatId;
extern mat4 modelMat;
extern mat4 viewMat;
extern mat4 projMat;

#endif