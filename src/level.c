#include "level.h"

#include <stdio.h>
#include <cglm/cglm.h>

#include "globals.h"
#include "obj_loader.h"
#include "render.h"
#include "input.h"

void loadLevel() {
    model = loadObj("assets/colors.obj");
    if (!model) {
        fprintf(stderr, "Error loading obj.\n");
        return;
    }

    glm_perspective(30.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f, projMat);
    glUniformMatrix4fv(projMatId, 1, GL_FALSE, &projMat[0][0]);

    vec3 eye = { 8.0f, 5.0f, 0.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, -1.0f, 0.0f };
    glm_lookat(eye, center, up, viewMat);
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &viewMat[0][0]);
}

void mainloop() {
    loadLevel();
 
    glEnableVertexAttribArray(ATTRIB_POSITION);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);

    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));
  
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

    glDisableVertexAttribArray(ATTRIB_POSITION);
    glDisableVertexAttribArray(ATTRIB_NORMAL);
    glDisableVertexAttribArray(ATTRIB_TEXCOORD);

    glDeleteBuffers(1, &model->vboId);
    free(model->faceFirst);
    free(model->faceCount);
}