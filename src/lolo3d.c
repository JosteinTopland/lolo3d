#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "types.h"
#include "globals.h"
#include "shader_loader.h"
#include "obj_loader.h"
#include "render.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Lolo 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    glewInit();
    if (!installShaders()) {
        fprintf(stderr, "Error loading shaders.\n");
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, windowWidth, windowHeight);

    Model *model = loadObj("assets/skull.obj");
    if (!model) {
        fprintf(stderr, "Error loading obj.\n");
        return 1;
    }

    glGenBuffers(1, &model->vboId);
    glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->numVertices, model->vertices, GL_STATIC_DRAW);
    free(model->vertices);

    glm_perspective(30.0f, (float) windowWidth / windowHeight, 0.1f, 100.0f, projMat);
    vec3 eye = { 3.0f, 0.0f, 0.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, -1.0f, 0.0f };
    glm_lookat(eye, center, up, viewMat);

    int run = 1;
    while (run) {
        int ticks = SDL_GetTicks();

        // input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode sym = event.key.keysym.sym;
                if (sym == SDLK_w) {
                    GLint polyMode[2];
                    glGetIntegerv(GL_POLYGON_MODE, polyMode);
                    glPolygonMode(GL_FRONT_AND_BACK, polyMode[0] == GL_FILL ? GL_LINE : GL_FILL);
                }
                if (sym == SDLK_q) {
                    run = 0;
                }
            }
        }

        render(model);
        SDL_GL_SwapWindow(window);

        // wait
        int fps = 30;
        int delay = 1000 / fps - (SDL_GetTicks() - ticks);
        if (delay > 0) SDL_Delay(delay);
    }

    free(model->faceFirst);
    free(model->faceCount);
    glDeleteBuffers(1, &model->vboId);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}