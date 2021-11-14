#include "obj_loader.h"

#include <SDL2/SDL.h>
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

    while (fgets(line, line_size, file)) {
        if (strstr(line, "map_Kd ")) {
            char* p = strchr(line, ' ') + 1;
            char path[100] = "assets/";
            strcat(path, p);
            *(path + strlen(path) - 1) = '\0';

            GLuint textureId;
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            SDL_Surface* image = SDL_LoadBMP(path);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
            SDL_FreeSurface(image);

            printf("%d=%s\n", textureId, path);
            // TODO store the filename + textureid
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

    int numObjects = 0;
    int numVertices = 0;
    int numNormals = 0;
    int numTexCoords = 0;
    int numFaces = 0;
    while (fgets(line, line_size, file)) {
        if (strstr(line, "o ")) numObjects++; // TODO nums below need to be reset for each new object, so that each object may have their own material etc
        if (strstr(line, "v ")) numVertices++;
        if (strstr(line, "vn ")) numNormals++;
        if (strstr(line, "vt ")) numTexCoords++;
        if (strstr(line, "f ")) numFaces++;
    }

    Model* model = malloc(sizeof(Model));
    model->numObjects = numObjects;
    model->numVertices = numFaces * 3;
    model->vertices = malloc(sizeof(Vertex) * model->numVertices);
    model->faceFirst = malloc(sizeof(GLint) * numFaces);
    model->faceCount = malloc(sizeof(GLsizei) * numFaces);
    model->numFaces = numFaces;

    GLfloat* vertices = malloc(sizeof(GLfloat) * numVertices * 3);
    GLfloat* normals = malloc(sizeof(GLfloat) * numNormals * 3);
    GLfloat* texCoords = malloc(sizeof(GLfloat) * numTexCoords * 2);

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
            char path[100] = "assets/";
            strcat(path, p);
            *(path + strlen(path) - 1) = '\0';
            loadMTL(path);
        }
        if (strstr(line, "o ")) {
            //TODO
        }
        if (strstr(line, "v ")) {
            char *p = strchr(line, ' ');
            GLfloat x = strtof(p, &p);
            GLfloat y = strtof(p, &p);
            GLfloat z = strtof(p, &p);
            *pv++ = x;
            *pv++ = y;
            *pv++ = z;
        }
        if (strstr(line, "vt ")) {
            char* p = strchr(line, ' ');
            GLfloat u = strtof(p, &p);
            GLfloat v = strtof(p, &p);
            *ptc++ = u;
            *ptc++ = v;
        }
        if (strstr(line, "vn ")) {
            char* p = strchr(line, ' ');
            GLfloat x = strtof(p, &p);
            GLfloat y = strtof(p, &p);
            GLfloat z = strtof(p, &p);
            *pn++ = x;
            *pn++ = y;
            *pn++ = z;
        }
        if (strstr(line, "usemtl ")) {
            //TODO
        }
        if (strstr(line, "f ")) {
            char* p = strchr(line, ' ');
            for (int i = 0; i < 3; i++) {
                GLsizei vIdx = 3 * (strtol(p + 1, &p, 10) - 1);
                GLsizei tIdx = 2 * (strtol(p + 1, &p, 10) - 1);
                GLsizei nIdx = 3 * (strtol(p + 1, &p, 10) - 1);
                pmv->position[0] = vertices[vIdx];
                pmv->position[1] = vertices[vIdx + 1];
                pmv->position[2] = vertices[vIdx + 2];
                pmv->textureCoord[0] = texCoords[tIdx];
                pmv->textureCoord[1] = 1 - texCoords[tIdx + 1];
                pmv->normal[0] = normals[nIdx];
                pmv->normal[1] = normals[nIdx + 1];
                pmv->normal[2] = normals[nIdx + 2];
                pmv++;
            }
            *pmff++ = faceFirst;
            *pmfc++ = 3;
            faceFirst += 3;
        }
    }
    free(line);
    fclose(file);

    free(vertices);
    free(normals);
    free(texCoords);

    glGenBuffers(1, &model->vboId);
    glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->numVertices, model->vertices, GL_STATIC_DRAW);
    free(model->vertices);

    model->material.diffuse[0] = 1.0f;
    model->material.diffuse[1] = 0.0f;
    model->material.diffuse[2] = 0.0f;
    model->material.diffuse[3] = 1.0f;

    return model;
}