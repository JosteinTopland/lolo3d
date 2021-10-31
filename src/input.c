#include "input.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "globals.h"

void input() {
    float angle = SDL_GetTicks() * 0.001f;
    vec3 pos = { 0.0f, 0.0f, 0.0f };
    vec3 axis = { 0.0f, 1.0f, 0.0f };
    glm_mat4_identity(modelMat);
    glm_translate(modelMat, pos);
    glm_rotate(modelMat, angle, axis);
    glUniformMatrix4fv(modelMatId, 1, GL_FALSE, &modelMat[0][0]);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode sym = event.key.keysym.sym;
            if (sym == SDLK_LEFT) cameraX += 0.1f;
            if (sym == SDLK_RIGHT) cameraX -= 0.1f;
            if (sym == SDLK_UP) cameraY += 0.1f;
            if (sym == SDLK_DOWN) cameraY -= 0.1f;
            if (sym == SDLK_w) {
                GLint polyMode[2];
                glGetIntegerv(GL_POLYGON_MODE, polyMode);
                glPolygonMode(GL_FRONT_AND_BACK, polyMode[0] == GL_FILL ? GL_LINE : GL_FILL);
            }
            if (sym == SDLK_q) {
                run = 0;
            }
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
            run = 0;
        }
    }
}