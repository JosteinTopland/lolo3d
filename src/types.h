#ifndef TYPES_H
#define TYPES_H

#include <GL/glew.h>

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
    GLuint textureId;
} Material;

typedef struct {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat textureCoord[2];
} Vertex;

typedef struct {
    GLuint vaoId;
    GLuint vboId;

    int numIndices;
    GLsizei* indices;
    Material* materials;

    int numMaterialLib;
    Material* materialLib;

    GLsizei numVertices;
    Vertex* vertices;
} Model;

typedef struct {
    GLfloat x, y, rotation;
    Model *model;
} Object;

typedef struct {
    int numObjects;
    Object *objects;
} Level;

#endif
