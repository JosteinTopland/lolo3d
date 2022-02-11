#include "shader_loader.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>

#include "types.h"
#include "globals.h"

char* read_file(const char *filename) {
    FILE* file;
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

int install_shaders() {
    GLuint programId = glCreateProgram();
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    GLint compileStatus;
    char* vertexShaderStr = read_file("src/vertex_shader.glsl");
    char* fragmentShaderStr = read_file("src/fragment_shader.glsl");
    const GLchar* vertexShaderStrs[] = { vertexShaderStr };
    const GLchar* fragmentShaderStrs[] = { fragmentShaderStr };

    glShaderSource(vertexShaderId, 1, vertexShaderStrs, NULL);
    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
      char info;
      int infoLen;
      glGetShaderInfoLog(vertexShaderId, 512, &infoLen, &info);
      fprintf(stderr, "Compile error on vertex shader:\n%.*s", infoLen, &info);
      return 0;
    }

    glShaderSource(fragmentShaderId, 1, fragmentShaderStrs, NULL);
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
      char info;
      int infoLen;
      glGetShaderInfoLog(fragmentShaderId, 512, &infoLen, &info);
      fprintf(stderr, "Compile error on fragment shader:\n%.*s", infoLen, &info);
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

    model_mat_id = glGetUniformLocation(programId, "modelMat");
    view_mat_id = glGetUniformLocation(programId, "viewMat");
    proj_mat_id = glGetUniformLocation(programId, "projMat");
    enable_texture = glGetUniformLocation(programId, "uEnableTexture");
    diffuse_color = glGetUniformLocation(programId, "uDiffuseColor");

    return 1;
}
