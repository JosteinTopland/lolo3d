#include "level.h"

#include <stdio.h>
#include <cglm/cglm.h>

#include "globals.h"
#include "obj_loader.h"
#include "render.h"
#include "input.h"

void loadLevel() {
    model = loadObj("assets/test.obj");
    if (!model) {
        fprintf(stderr, "Error loading obj.\n");
        return;
    }

    glm_perspective(30.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f, projMat);
    glUniformMatrix4fv(projMatId, 1, GL_FALSE, &projMat[0][0]);

    vec3 eye = { 5.0f, 3.0f, 0.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, -1.0f, 0.0f };
    glm_lookat(eye, center, up, viewMat);
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &viewMat[0][0]);
}

void mainloop() {
    loadLevel();
   
    while (run) {
        int ticks = SDL_GetTicks();

        input();
        render(model);
        SDL_GL_SwapWindow(window);

        // wait
        int fps = 100;
        int delay = 1000 / fps - (SDL_GetTicks() - ticks);
        if (delay > 0) SDL_Delay(delay);
    }

    freeModel(model);
}