#ifndef TYPES_H
#define TYPES_H

#include <GL/glew.h>

enum {
    ATTRIB_POSITION,
    ATTRIB_NORMAL,
    ATTRIB_TEXCOORD
};
typedef struct {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    char texture[50];
} Material;

typedef struct {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat textureCoord[2];
} Vertex;

typedef struct {
    GLuint vboId;
    GLsizeiptr numVertices;
    GLsizei numNormals;
    GLsizei numTexCoords;
    GLsizei numFaces;
    Vertex* vertices;
    GLint* faceFirst;
    GLsizei* faceCount;
    Material material;
} Group;

typedef struct {
    Group* groups;
    int numGroups;
} Model;

#endif