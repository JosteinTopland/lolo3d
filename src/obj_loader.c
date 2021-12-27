#include "obj_loader.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* strtrim(char *s) {
    while (isspace(*s)) s++;
    if (!strlen(s)) return s;
    char* p = s + strlen(s);
    while (isspace(*--p));
    *(p + 1) = '\0';
    return s;
}

int begins_with(const char* pre, char* str) {
    return !strncmp(pre, strtrim(str), strlen(pre));
}

void loadMTL(const char* filename, Model* model) {
    FILE* file;
    file = fopen(filename, "rb");
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
            strtrim(pm->name);
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
            strtrim(path);

            glGenTextures(1, &pm->textureId);
            glBindTexture(GL_TEXTURE_2D, pm->textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            SDL_Surface* image = IMG_Load(path);
            GLenum format = (image->format->format == SDL_PIXELFORMAT_RGB24) ? GL_RGB : GL_BGR;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
            SDL_FreeSurface(image);
        }
    }
    free(line);
    fclose(file);
}

Model *loadObj(const char *filename) {
    FILE* file;
    file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error loading obj.\n");
        return 0;
    }

    const int line_size = 300;
    char* line = malloc(line_size);
    if (!line) return 0;

    Model* model = malloc(sizeof(Model));
    int numVertices = 0;
    int numNormals = 0;
    int numTexCoords = 0;
    int numFaces = 0;
    int numMaterials = 0;

    while (fgets(line, line_size, file)) {
        if (begins_with("mtllib ", line)) {
            char* p = strchr(line, ' ') + 1;
            char path[100];
            strcpy(path, "assets/");
            strcat(path, p);
            strtrim(path);
            loadMTL(path, model);
        }
        if (begins_with("v ", line)) numVertices++;
        if (begins_with("vn ", line)) numNormals++;
        if (begins_with("vt ", line)) numTexCoords++;
        if (begins_with("f ", line)) numFaces++;
        if (begins_with("usemtl ", line)) numMaterials++;
    }

    model->numIndices = numMaterials;
    model->numVertices = numFaces * 3; // triangular faces only, since GL_POLYGONS is not supported after OpenGL 3.1
    model->indices = malloc(sizeof(GLsizei) * model->numIndices);
    model->materials = malloc(sizeof(Material) * model->numIndices);
    model->vertices = malloc(sizeof(Vertex) * model->numVertices);
    GLsizei* pi = model->indices;
    Material* pm = model->materials;
    Vertex* pvv = model->vertices;

    GLfloat* vertices = malloc(sizeof(GLfloat) * numVertices * 3);
    GLfloat* normals = malloc(sizeof(GLfloat) * numNormals * 3);
    GLfloat* texCoords = malloc(sizeof(GLfloat) * numTexCoords * 2);
    GLfloat* pv = vertices;
    GLfloat* pn = normals;
    GLfloat* ptc = texCoords;

    int firstMaterial = 1;
    int numIndices = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
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
            strtrim(name);
            for (int i = 0; i < model->numMaterialLib; i++) {
                int res = strcmp(model->materialLib[i].name, name);
                if (!res) {
                    *pm = model->materialLib[i];
                }
            }
            pm++;

            if (!firstMaterial) *pi++ = numIndices;
            firstMaterial = 0;
            numIndices = 0;
        }
        if (begins_with("f ", line)) {
            char* p = strchr(line, ' ');
            for (int i = 0; i < 3; i++) {
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

    glGenVertexArrays(1, &model->vaoId);
    glBindVertexArray(model->vaoId);

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