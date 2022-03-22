#include "globals.h"

SDL_Window* window;
const int window_width = 800;
const int window_height = 600;

int run = 1;
GLfloat camera_x = 0;
GLfloat camera_y = -3.0f;

GLint model_mat_id;
GLint view_mat_id;
GLint proj_mat_id;
GLint enable_texture;
GLint diffuse_color;

mat4 model_mat;
mat4 view_mat;
mat4 proj_mat;
