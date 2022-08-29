#include "globals.h"

char run = 1;
GLfloat cam_x = 0.0f;
GLfloat cam_y = 30.0f;

GLint model_mat_id;
GLint view_mat_id;
GLint proj_mat_id;
GLint enable_texture;
GLint diffuse_color;

mat4 model_mat;
mat4 view_mat;
mat4 proj_mat;
