#ifndef DATA_H
#define DATA_H

#include <cglm/cglm.h>
#include <GL/glew.h>

extern char run;
extern GLfloat cam_x;
extern GLfloat cam_y;

extern GLint model_mat_id;
extern GLint view_mat_id;
extern GLint proj_mat_id;
extern GLint enable_texture;
extern GLint diffuse_color;

extern mat4 model_mat;
extern mat4 view_mat;
extern mat4 proj_mat;

#endif
