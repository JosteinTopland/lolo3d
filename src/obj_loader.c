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

void load_mtl(const char* filename, Model* model) {
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

    model->num_material_lib = numMaterials;
    model->material_lib = malloc(sizeof(Material) * model->num_material_lib);
    Material* pm = model->material_lib;

    int firstMaterial = 1;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (begins_with("newmtl ", line)) {
            if (!firstMaterial) pm++;
            firstMaterial = 0;

            char* p = strchr(line, ' ') + 1;
            strcpy(pm->name, p);
            strtrim(pm->name);
            pm->texture_id = 0;
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

            glGenTextures(1, &pm->texture_id);
            glBindTexture(GL_TEXTURE_2D, pm->texture_id);
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

Model *load_obj(const char *filename) {
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
            load_mtl(path, model);
        }
        if (begins_with("v ", line)) numVertices++;
        if (begins_with("vn ", line)) numNormals++;
        if (begins_with("vt ", line)) numTexCoords++;
        if (begins_with("f ", line)) numFaces++;
        if (begins_with("usemtl ", line)) numMaterials++;
    }

    model->num_indices = numMaterials;
    model->num_vertices = numFaces * 3; // triangular faces only, since GL_POLYGONS is not supported after OpenGL 3.1
    model->indices = malloc(sizeof(GLsizei) * model->num_indices);
    model->materials = malloc(sizeof(Material) * model->num_indices);
    model->vertices = malloc(sizeof(Vertex) * model->num_vertices);
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
            for (int i = 0; i < model->num_material_lib; i++) {
                int res = strcmp(model->material_lib[i].name, name);
                if (!res) {
                    *pm = model->material_lib[i];
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
                pvv->texcoord[0] = texCoords[tIdx];
                pvv->texcoord[1] = 1 - texCoords[tIdx + 1];
                pvv++;
                numIndices++;
            }
        }
    }
    *pi = numIndices;
    free(line);
    fclose(file);

    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);

    glGenBuffers(1, &model->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->num_vertices, model->vertices, GL_STATIC_DRAW);
    free(model->vertices);

    return model;
}

void free_model(Model* model) {
    glDeleteBuffers(1, &model->vbo);
    free(model->materials);
    free(model->indices);
    free(model->material_lib);
}
