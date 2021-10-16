#include "shader_loader.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>

#include "types.h"
#include "globals.h"

char* readFile(const char *filename) {
    FILE* file;
    //fopen_s(&file, filename, "rb");
    file = fopen(filename, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *strOut = malloc(fsize + 1);
        if (strOut) {
            fread(strOut, 1, fsize + 1, file);
            strOut[fsize] = 0;
        }
        fclose(file);
        return strOut;
    }
    return 0;
}

int installShaders() {
    GLuint programId = glCreateProgram();
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    GLint compileStatus;
    char* vertexShaderStr = readFile("src/vertex_shader.glsl");
    char* fragmentShaderStr = readFile("src/fragment_shader.glsl");
    const GLchar* vertexShaderStrs[] = { vertexShaderStr };
    const GLchar* fragmentShaderStrs[] = { fragmentShaderStr };

    glShaderSource(vertexShaderId, 1, vertexShaderStrs, NULL);
    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
      fprintf(stderr, "Compile error on vertex shader.\n");
      return 0;
    }

    glShaderSource(fragmentShaderId, 1, fragmentShaderStrs, NULL);
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
      fprintf(stderr, "Compile error on fragment shader.\n");
      return 0;
    }

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glBindAttribLocation(programId, ATTRIB_POSITION, "aPosition");
    glBindAttribLocation(programId, ATTRIB_NORMAL, "aNormal");
    glBindAttribLocation(programId, ATTRIB_TEXCOORD, "aTexCoord");

    glLinkProgram(programId);

    GLint linkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
      fprintf(stderr, "Link error on shaders.\n");
      return 0;
    }

    free(vertexShaderStr);
    free(fragmentShaderStr);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    
    glUseProgram(programId);

    modelMatId = glGetUniformLocation(programId, "modelMat");
    viewMatId = glGetUniformLocation(programId, "viewMat");
    projMatId = glGetUniformLocation(programId, "projMat");

    return 1;
}