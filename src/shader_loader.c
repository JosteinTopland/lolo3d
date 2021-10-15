#include "shader_loader.h"

#include <SDL2/SDL.h>
#include <gl/glew.h>
#include <stdio.h>

char* readFile(const char *filename) {
    FILE* file;
    fopen_s(&file, filename, "rb");
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
    GLchar* shaderStrs[1];

    shaderStrs[0] = readFile("src/vertex_shader.glsl");
    if (shaderStrs[0]) {
        glShaderSource(vertexShaderId, 1, shaderStrs, NULL);
        //free(shaderStrs[0]);
        glCompileShader(vertexShaderId);
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE) return 0;
    } else {
        return 0;
    }

    shaderStrs[0] = readFile("src/fragment_shader.glsl");
    if (shaderStrs[0]) {
        glShaderSource(fragmentShaderId, 1, shaderStrs, NULL);
        glCompileShader(fragmentShaderId);
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE) return 0;
    } else {
        return 0;
    }

    return 1;
}