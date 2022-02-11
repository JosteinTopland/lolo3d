#ifndef DATA_H
#define DATA_H

#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <GL/glew.h>

SDL_Window* window;
const int window_width;
const int window_height;

int run;
GLfloat camera_x;
GLfloat camera_y;

GLint model_mat_id;
GLint view_mat_id;
GLint proj_mat_id;
GLint enable_texture;
GLint diffuse_color;

extern mat4 model_mat;
extern mat4 view_mat;
extern mat4 proj_mat;

#endif
