#ifndef TYPES_H
#define TYPES_H

#include <GL/glew.h>

enum {
    ATTRIB_POSITION,
    ATTRIB_NORMAL,
    ATTRIB_TEXCOORD
};

typedef struct {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat textureCoord[2];
} Vertex;

typedef struct {
    GLuint vboId;
    GLsizeiptr numVertices;
    GLsizei numFaces;
    Vertex* vertices;
    GLint* faceFirst;
    GLsizei* faceCount;
} Model;

#endif