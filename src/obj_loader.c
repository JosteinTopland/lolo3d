#include "obj_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadMTL(const char* filename) {
    FILE* file;
#ifdef _WIN32
    fopen_s(&file, filename, "rb");
#else
    file = fopen(filename, "rb");
#endif
    if (!file) return;

    const int line_size = 300;
    char* line = malloc(line_size);
    if (!line) return;

    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (strstr(line, "map_Kd ")) {
            char* p = strchr(line, ' ') + 1;
            printf("%s", p);
        }
    }

    free(line);
    fclose(file);
}

Model *loadObj(const char *filename) {
    FILE* file;
#ifdef _WIN32
    fopen_s(&file, filename, "rb");
#else
    file = fopen(filename, "rb");
#endif
    if (!file) return 0;

    const int line_size = 300;
    char* line = malloc(line_size);
    if (!line) return 0;

    int numVertices = 0;
    int numNormals = 0;
    int numTexCoords = 0;
    int numFaces = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (strstr(line, "v ")) numVertices++;
        if (strstr(line, "vn ")) numNormals++;
        if (strstr(line, "vt ")) numTexCoords++;
        if (strstr(line, "f ")) numFaces++;
    }

    GLfloat* vertices = malloc(sizeof(GLfloat) * numVertices * 3);
    GLfloat* texCoords = malloc(sizeof(GLfloat) * numTexCoords * 2);
    GLfloat* normals = malloc(sizeof(GLfloat) * numNormals * 3);
    Model* model = malloc(sizeof(Model));
    model->numVertices = numFaces * 3;
    model->numFaces = numFaces;
    model->vertices = malloc(sizeof(Vertex) * model->numVertices);
    model->faceFirst = malloc(sizeof(GLint) * model->numFaces);
    model->faceCount = malloc(sizeof(GLsizei) * model->numFaces);

    GLfloat* pv = vertices;
    GLfloat* ptc = texCoords;
    GLfloat* pn = normals;
    Vertex* pmv = model->vertices;
    GLint* pmff = model->faceFirst;
    GLsizei* pmfc = model->faceCount;

    GLint faceFirst = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (strstr(line, "mtllib ")) {
            char* p = strchr(line, ' ') + 1;
            loadMTL("assets/cube.mtl");
        }
        if (strstr(line, "v ")) {
            char *p = strchr(line, ' ');
            GLfloat x = strtof(p, &p);
            GLfloat y = strtof(p, &p);
            GLfloat z = strtof(p, NULL);
            *pv++ = x;
            *pv++ = y;
            *pv++ = z;
        }
        if (strstr(line, "vt ")) {
            char* p = strchr(line, ' ');
            GLfloat u = strtof(p, &p);
            GLfloat v = strtof(p, NULL);
            *ptc++ = u;
            *ptc++ = v;
        }
        if (strstr(line, "vn ")) {
            char* p = strchr(line, ' ');
            GLfloat x = strtof(p, &p);
            GLfloat y = strtof(p, &p);
            GLfloat z = strtof(p, NULL);
            *pn++ = x;
            *pn++ = y;
            *pn++ = z;
        }
        if (strstr(line, "f ")) {
            char* p = strchr(line, ' ') + 1;
            GLsizei vIdx1 = strtol(p, &p, 10);
            GLsizei tIdx1 = strtol(p + 1, &p, 10);
            GLsizei nIdx1 = strtol(p + 1, &p, 10);
            GLsizei vIdx2 = strtol(p + 1, &p, 10);
            GLsizei tIdx2 = strtol(p + 1, &p, 10);
            GLsizei nIdx2 = strtol(p + 1, &p, 10);
            GLsizei vIdx3 = strtol(p + 1, &p, 10);
            GLsizei tIdx3 = strtol(p + 1, &p, 10);
            GLsizei nIdx3 = strtol(p + 1, NULL, 10);
            
            pmv->position[0] = vertices[3 * (vIdx1 - 1)];
            pmv->position[1] = vertices[3 * (vIdx1 - 1) + 1];
            pmv->position[2] = vertices[3 * (vIdx1 - 1) + 2];
            pmv->textureCoord[0] = texCoords[2 * (tIdx1 - 1)];
            pmv->textureCoord[1] = texCoords[2 * (tIdx1 - 1) + 1];
            pmv->normal[0] = normals[3 * (nIdx1 - 1)];
            pmv->normal[1] = normals[3 * (nIdx1 - 1) + 1];
            pmv->normal[2] = normals[3 * (nIdx1 - 1) + 2];
            pmv++;

            pmv->position[0] = vertices[3 * (vIdx2 - 1)];
            pmv->position[1] = vertices[3 * (vIdx2 - 1) + 1];
            pmv->position[2] = vertices[3 * (vIdx2 - 1) + 2];
            pmv->textureCoord[0] = texCoords[2 * (tIdx2 - 1)];
            pmv->textureCoord[1] = texCoords[2 * (tIdx2 - 1) + 1];
            pmv->normal[0] = normals[3 * (nIdx2 - 1)];
            pmv->normal[1] = normals[3 * (nIdx2 - 1) + 1];
            pmv->normal[2] = normals[3 * (nIdx2 - 1) + 2];
            pmv++;

            pmv->position[0] = vertices[3 * (vIdx3 - 1)];
            pmv->position[1] = vertices[3 * (vIdx3 - 1) + 1];
            pmv->position[2] = vertices[3 * (vIdx3 - 1) + 2];
            pmv->textureCoord[0] = texCoords[2 * (tIdx3 - 1)];
            pmv->textureCoord[1] = texCoords[2 * (tIdx3 - 1) + 1];
            pmv->normal[0] = normals[3 * (nIdx3 - 1)];
            pmv->normal[1] = normals[3 * (nIdx3 - 1) + 1];
            pmv->normal[2] = normals[3 * (nIdx3 - 1) + 2];
            pmv++;

            *pmff++ = faceFirst;
            *pmfc++ = 3;
            faceFirst += 3;
        }
    }
    free(line);
    fclose(file);

    free(vertices);
    free(texCoords);
    free(normals);

    return model;
}