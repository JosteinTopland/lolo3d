#include "obj_loader.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int begins_with(const char* pre, const char* str) {
    return !strncmp(pre, str, strlen(pre));
}

void loadMTL(const char* filename, Model* model) {
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

    int numMaterials = 0;
    while (fgets(line, line_size, file)) {
        if (begins_with("newmtl ", line)) numMaterials++;
    }

    model->numMaterialLib = numMaterials;
    model->materialLib = malloc(sizeof(Material) * model->numMaterialLib);
    Material* pm = model->materialLib;

    int firstMaterial = 1;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (begins_with("newmtl ", line)) {
            if (!firstMaterial) pm++;
            firstMaterial = 0;

            char* p = strchr(line, ' ') + 1;
            strcpy(pm->name, p);
            pm->name[strlen(pm->name) - 1] = '\0';
            pm->textureId = 0;
        }
        if (begins_with("Kd ", line)) {
            char* p = strchr(line, ' ');
            GLfloat r = strtof(p, &p);
            GLfloat g = strtof(p, &p);
            GLfloat b = strtof(p, &p);
            pm->diffuse[0] = r;
            pm->diffuse[1] = g;
            pm->diffuse[2] = b;
        }
        if (begins_with("map_Kd ", line)) {
            char* p = strchr(line, ' ') + 1;
            char path[100];
            strcpy(path, "assets/");
            strcat(path, p);
            path[strlen(path) - 1] = '\0';

            glGenTextures(1, &pm->textureId);
            glBindTexture(GL_TEXTURE_2D, pm->textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            SDL_Surface* image = SDL_LoadBMP(path);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
            SDL_FreeSurface(image);
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

    Model* model = malloc(sizeof(Model));
    int numGroups = 0;
    int numVertices = 0;
    int numNormals = 0;
    int numTexCoords = 0;
    int numFaces = 0;
    while (fgets(line, line_size, file)) {
        if (begins_with("mtllib ", line)) {
            char* p = strchr(line, ' ') + 1;
            char path[100];
            strcpy(path, "assets/");
            strcat(path, p);
            path[strlen(path) - 1] = '\0';
            loadMTL(path, model);
        }
        if (begins_with("g ", line) || begins_with("o ", line)) numGroups++;
        if (begins_with("v ", line)) numVertices++;
        if (begins_with("vn ", line)) numNormals++;
        if (begins_with("vt ", line)) numTexCoords++;
        if (begins_with("f ", line)) {
            char* p = strchr(line, ' ');
            for (int i = 0; strtol(p + 1, &p, 10); i++) {
                if (i % 3 == 0) numFaces++;
            }
        }
    }

    model->numGroups = numGroups;
    model->numVertices = numFaces;
    model->materials = malloc(sizeof(Material) * model->numGroups);
    model->indices = malloc(sizeof(GLsizei) * model->numGroups);
    model->vertices = malloc(sizeof(Vertex) * model->numVertices);
    Material* pm = model->materials;
    GLsizei* pi = model->indices;
    Vertex* pvv = model->vertices;

    GLfloat* vertices = malloc(sizeof(GLfloat) * numVertices * 3);
    GLfloat* normals = malloc(sizeof(GLfloat) * numNormals * 3);
    GLfloat* texCoords = malloc(sizeof(GLfloat) * numTexCoords * 2);
    GLfloat* pv = vertices;
    GLfloat* pn = normals;
    GLfloat* ptc = texCoords;

    int firstGroup = 1;
    GLsizei numIndices = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (begins_with("g ", line) || begins_with("o ", line)) {
            if (!firstGroup) {
                *pi++ = numIndices;                
            }
            firstGroup = 0;
            numIndices = 0;
        }
        if (begins_with("v ", line)) {
            char *p = strchr(line, ' ');
            GLfloat x = strtof(p, &p);
            GLfloat y = strtof(p, &p);
            GLfloat z = strtof(p, &p);
            *pv++ = x;
            *pv++ = y;
            *pv++ = z;
        }
        if (begins_with("vn ", line)) {
            char* p = strchr(line, ' ');
            GLfloat x = strtof(p, &p);
            GLfloat y = strtof(p, &p);
            GLfloat z = strtof(p, &p);
            *pn++ = x;
            *pn++ = y;
            *pn++ = z;
        }
        if (begins_with("vt ", line)) {
            char* p = strchr(line, ' ');
            GLfloat u = strtof(p, &p);
            GLfloat v = strtof(p, &p);
            *ptc++ = u;
            *ptc++ = v;
        }
        if (begins_with("usemtl ", line)) {
            char* p = strchr(line, ' ') + 1;
            char name[100];
            strcpy(name, p);
            name[strlen(name) - 1] = '\0';

            for (int i = 0; i < model->numMaterialLib; i++) {
                int res = strcmp(model->materialLib[i].name, name);
                if (!res) {
                    *pm = model->materialLib[i];
                }
            }
            pm++;
        }
        if (begins_with("f ", line)) {
            char* p = strchr(line, ' ');
            while (strtol(p + 1, NULL, 10)) {
                GLsizei vIdx = 3 * (strtol(p + 1, &p, 10) - 1);
                GLsizei tIdx = 2 * (strtol(p + 1, &p, 10) - 1);
                GLsizei nIdx = 3 * (strtol(p + 1, &p, 10) - 1);
                pvv->position[0] = vertices[vIdx];
                pvv->position[1] = vertices[vIdx + 1];
                pvv->position[2] = vertices[vIdx + 2];
                pvv->normal[0] = normals[nIdx];
                pvv->normal[1] = normals[nIdx + 1];
                pvv->normal[2] = normals[nIdx + 2];
                pvv->textureCoord[0] = texCoords[tIdx];
                pvv->textureCoord[1] = 1 - texCoords[tIdx + 1];
                pvv++;
                numIndices++;
            }
        }
    }
    *pi = numIndices;
    free(line);
    fclose(file);

    glGenBuffers(1, &model->vboId);
    glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->numVertices, model->vertices, GL_STATIC_DRAW);
    free(model->vertices);

    return model;
}

void freeModel(Model* model) {
    glDeleteBuffers(1, &model->vboId);
    free(model->materials);
    free(model->indices);
    free(model->materialLib);
}