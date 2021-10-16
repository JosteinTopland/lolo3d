#include "render.h"

#include <SDL2/SDL.h>
#include "globals.h"

void render(Model *model) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));

    vec3 pos = { 0.0f, 0.0f, 0.0f };
    float angle = SDL_GetTicks() * 0.0005f;
    vec3 axis = { 0.0f, 1.0f, 0.0f };
    glm_mat4_identity(modelMat);
    glm_translate(modelMat, pos);
    glm_rotate(modelMat, angle, axis);

    glUniformMatrix4fv(modelMatId, 1, GL_FALSE, &modelMat[0][0]);
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(projMatId, 1, GL_FALSE, &projMat[0][0]);

    glMultiDrawArrays(GL_TRIANGLES, model->faceFirst, model->faceCount, model->numFaces);

    glDisableVertexAttribArray(ATTRIB_POSITION);
    glDisableVertexAttribArray(ATTRIB_NORMAL);
    glDisableVertexAttribArray(ATTRIB_TEXCOORD);
}