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
        if (strstr(line, "Kd ")) {
        }
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

    int numGroups = 0;
    while (fgets(line, line_size, file)) {
        if (strstr(line, "g ") || strstr(line, "o ")) {
            numGroups++;
        }
    }
    Model* model = malloc(sizeof(Model));
    model->numGroups = numGroups;
    model->groups = malloc(sizeof(Group) * model->numGroups);

    int groupIdx = 0;
    int numVertices = 0;
    int numNormals = 0;
    int numTexCoords = 0;
    int numFaces = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (strstr(line, "g ") || strstr(line, "o ")) {
            if (groupIdx > 0) {
                Group* group = &model->groups[groupIdx - 1];
                group->numVertices = numFaces * 3; // 3 points per face obj format only! 
                group->numNormals = numNormals;
                group->numTexCoords = numTexCoords;
                group->numFaces = numFaces;
                group->vertices = malloc(sizeof(Vertex) * group->numVertices);
                group->faceFirst = malloc(sizeof(GLint) * group->numFaces);
                group->faceCount = malloc(sizeof(GLsizei) * group->numFaces);
            }
            groupIdx++;
            numVertices = 0;
            numNormals = 0;
            numTexCoords = 0;
            numFaces = 0;
        }
        if (strstr(line, "v ")) numVertices++;
        if (strstr(line, "vn ")) numNormals++;
        if (strstr(line, "vt ")) numTexCoords++;
        if (strstr(line, "f ")) numFaces++;
    }
    // TODO last line. clean up
    Group* group = &model->groups[groupIdx - 1];
    group->numVertices = numFaces * 3; // 3 points per face obj format only! 
    group->numNormals = numNormals;
    group->numTexCoords = numTexCoords;
    group->numFaces = numFaces;
    group->vertices = malloc(sizeof(Vertex) * group->numVertices);
    group->faceFirst = malloc(sizeof(GLint) * group->numFaces);
    group->faceCount = malloc(sizeof(GLsizei) * group->numFaces);

    GLfloat* vertices = NULL;
    GLfloat* normals = NULL;
    GLfloat* texCoords = NULL;
    GLfloat* pv;
    GLfloat* pn;
    GLfloat* ptc;
    Vertex* pgv;
    GLint* pgff;
    GLsizei* pgfc;
    GLint faceFirst = 0;

    groupIdx = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, line_size, file)) {
        if (0 && strstr(line, "mtllib ")) {
            char* p = strchr(line, ' ') + 1;
            char path[100] = "assets/";
            strcat(path, p);
            *(path + strlen(path) - 1) = '\0';
            loadMTL(path);
        }
        if (strstr(line, "g ") || strstr(line, "o ")) {
            if (vertices != NULL) free(vertices);
            if (normals != NULL) free(normals);
            if (texCoords != NULL) free(texCoords);

            vertices = malloc(sizeof(GLfloat) * model->groups[groupIdx].numVertices * 3);
            normals = malloc(sizeof(GLfloat) * model->groups[groupIdx].numNormals * 3);
            texCoords = malloc(sizeof(GLfloat) * model->groups[groupIdx].numTexCoords * 2);
            pv = vertices;
            pn = normals;
            ptc = texCoords;

            pgv = model->groups[groupIdx].vertices;
            pgff = model->groups[groupIdx].faceFirst;
            pgfc = model->groups[groupIdx].faceCount;

            //faceFirst = 0;
            groupIdx++;
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
        if (strstr(line, "vn ")) {
            char* p = strchr(line, ' ');
            GLfloat x = strtof(p, &p);
            GLfloat y = strtof(p, &p);
            GLfloat z = strtof(p, &p);
            *pn++ = x;
            *pn++ = y;
            *pn++ = z;
        }
        if (strstr(line, "vt ")) {
            char* p = strchr(line, ' ');
            GLfloat u = strtof(p, &p);
            GLfloat v = strtof(p, &p);
            *ptc++ = u;
            *ptc++ = v;
        }
        if (strstr(line, "usemtl ")) {
            model->groups[groupIdx - 1].material.diffuse[0] = 0.0f;
            model->groups[groupIdx - 1].material.diffuse[1] = 1.0f;
            model->groups[groupIdx - 1].material.diffuse[2] = 0.0f;
            model->groups[groupIdx - 1].material.diffuse[3] = 1.0f;
        }
        if (strstr(line, "f ")) {
            char* p = strchr(line, ' ');
            for (int i = 0; i < 3; i++) {
                GLsizei vIdx = 3 * (strtol(p + 1, &p, 10) - 1);
                GLsizei tIdx = 2 * (strtol(p + 1, &p, 10) - 1);
                GLsizei nIdx = 3 * (strtol(p + 1, &p, 10) - 1);
                pgv->position[0] = vertices[vIdx];
                pgv->position[1] = vertices[vIdx + 1];
                pgv->position[2] = vertices[vIdx + 2];
                pgv->normal[0] = normals[nIdx];
                pgv->normal[1] = normals[nIdx + 1];
                pgv->normal[2] = normals[nIdx + 2];
                pgv->textureCoord[0] = texCoords[tIdx];
                pgv->textureCoord[1] = 1 - texCoords[tIdx + 1];
                pgv++;
            }
            *pgff++ = faceFirst;
            *pgfc++ = 3;
            faceFirst += 3;
        }
    }
    free(line);
    fclose(file);

    for (int i = 0; i < model->numGroups; i++) {
        glGenBuffers(1, &model->groups[i].vboId);
        glBindBuffer(GL_ARRAY_BUFFER, model->groups[i].vboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->groups[i].numVertices, model->groups[i].vertices, GL_STATIC_DRAW);
        free(model->groups[i].vertices);
    }

    return model;
}