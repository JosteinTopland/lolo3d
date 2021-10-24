#include "render.h"

#include <SDL2/SDL.h>
#include "globals.h"

void render(Model *model) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float angle = SDL_GetTicks() * 0.001f;
    vec3 pos = { 0.0f, 0.0f, 0.0f };
    vec3 axis = { 0.0f, 1.0f, 0.0f };
    glm_mat4_identity(modelMat);
    glm_translate(modelMat, pos);
    glm_rotate(modelMat, angle, axis);
    glUniformMatrix4fv(modelMatId, 1, GL_FALSE, &modelMat[0][0]);

    glMultiDrawArrays(GL_TRIANGLES, model->faceFirst, model->faceCount, model->numFaces);
}