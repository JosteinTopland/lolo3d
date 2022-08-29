#ifndef TYPES_H
#define TYPES_H

#include <GL/glew.h>
#include <SDL2/SDL_mixer.h>

enum {
    ATTRIB_POSITION,
    ATTRIB_NORMAL,
    ATTRIB_TEXCOORD
};
typedef struct {
    char name[100];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    GLuint texture_id;
} Material;

typedef struct {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texcoord[2];
} Vertex;

typedef struct {
    GLuint vao;
    GLuint vbo;

    int num_indices;
    GLsizei* indices;
    Material* materials;

    int num_material_lib;
    Material* material_lib;

    GLsizei num_vertices;
    Vertex* vertices;
} Model;

typedef struct {
    GLfloat x, y, rotation;
    Model *model;
} Object;

typedef struct {
    int num_models;
    Model **models;
    int num_objects;
    Object *objects;
    Mix_Music *music;
} Level;

#endif
