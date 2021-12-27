#include "render.h"

#include "globals.h"

void render_level(Level *level) {
    glClearColor(0.1f, 0.1f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < level->numObjects; i++) {
        if (!level->objects[i].model) continue;
        Object *object = &level->objects[i];
        Model *model = object->model;

        vec3 pos = { object->x, 0, object->y };
        glm_mat4_identity(modelMat);
        glm_translate(modelMat, pos);
        glm_rotate(modelMat, object->rotation, GLM_YUP);
        glUniformMatrix4fv(modelMatId, 1, GL_FALSE, &modelMat[0][0]);
        
        glBindVertexArray(model->vaoId);
        glBindBuffer(GL_ARRAY_BUFFER, model->vboId);

        glEnableVertexAttribArray(ATTRIB_POSITION);
        glEnableVertexAttribArray(ATTRIB_NORMAL);
        glEnableVertexAttribArray(ATTRIB_TEXCOORD);

        glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
        glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));

        GLint first = 0;
        for (int j = 0; j < model->numIndices; j++) {
            if (model->materials[j].textureId) glBindTexture(GL_TEXTURE_2D, model->materials[j].textureId);
            glUniform1i(enableTexture, model->materials[j].textureId);
            glUniform4fv(diffuseColor, 1, &model->materials[j].diffuse[0]);
            glDrawArrays(GL_TRIANGLES, first, model->indices[j]);
            first += model->indices[j];
        }

        glDisableVertexAttribArray(ATTRIB_POSITION);
        glDisableVertexAttribArray(ATTRIB_NORMAL);
        glDisableVertexAttribArray(ATTRIB_TEXCOORD);
    }
}