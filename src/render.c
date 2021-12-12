#include "render.h"

#include "globals.h"

void render_level(Model *model) {
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float angle = SDL_GetTicks() * 0.002;
    vec3 pos = { -3, sin(angle), 0 };
    glm_mat4_identity(modelMat);
    glm_translate(modelMat, pos);
    glm_rotate(modelMat, sin(angle) * 0.5, GLM_YUP);
    glUniformMatrix4fv(modelMatId, 1, GL_FALSE, &modelMat[0][0]);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);

    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));

    GLint first = 0;
    glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
    for (int i = 0; i < model->numIndices; i++) {
        glBindTexture(GL_TEXTURE_2D, model->materials[i].textureId);
        glUniform1i(enableTexture, model->materials[i].textureId);
        glUniform4fv(diffuseColor, 1, &model->materials[i].diffuse[0]);
        glDrawArrays(GL_TRIANGLES, first, model->indices[i]);
        first += model->indices[i];
    }

    float angle2 = SDL_GetTicks() * 0.005;
    vec3 pos2 = { 3, cos(angle2), 0 };
    glm_mat4_identity(modelMat);
    glm_translate(modelMat, pos2);
    glm_rotate(modelMat, sin(angle2) * 0.5, GLM_YUP);
    glUniformMatrix4fv(modelMatId, 1, GL_FALSE, &modelMat[0][0]);

    first = 0;
    for (int i = 0; i < model->numIndices; i++) {
        glBindTexture(GL_TEXTURE_2D, model->materials[i].textureId);
        glUniform1i(enableTexture, model->materials[i].textureId);
        glUniform4fv(diffuseColor, 1, &model->materials[i].diffuse[0]);
        glDrawArrays(GL_TRIANGLES, first, model->indices[i]);
        first += model->indices[i];
    }

    glDisableVertexAttribArray(ATTRIB_POSITION);
    glDisableVertexAttribArray(ATTRIB_NORMAL);
    glDisableVertexAttribArray(ATTRIB_TEXCOORD);
}