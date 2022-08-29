#ifndef DATA_H
#define DATA_H

#include <cglm/cglm.h>
#include <GL/glew.h>

char run;
GLfloat cam_x;
GLfloat cam_y;

GLint model_mat_id;
GLint view_mat_id;
GLint proj_mat_id;
GLint enable_texture;
GLint diffuse_color;

extern mat4 model_mat;
extern mat4 view_mat;
extern mat4 proj_mat;

#endif
