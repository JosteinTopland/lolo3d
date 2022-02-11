#include "render.h"

#include "globals.h"

void render_level(Level *level) {
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < level->num_objects; i++) {
        if (!level->objects[i].model) continue;
        Object *object = &level->objects[i];
        Model *model = object->model;

        vec3 pos = { object->x, object->y, 0 };
        glm_mat4_identity(model_mat);
        glm_translate(model_mat, pos);
        glm_rotate(model_mat, object->rotation, GLM_YUP);
        glUniformMatrix4fv(model_mat_id, 1, GL_FALSE, &model_mat[0][0]);
        
        glBindVertexArray(model->vao);
        glBindBuffer(GL_ARRAY_BUFFER, model->vbo);

        glEnableVertexAttribArray(ATTRIB_POSITION);
        glEnableVertexAttribArray(ATTRIB_NORMAL);
        glEnableVertexAttribArray(ATTRIB_TEXCOORD);

        glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
        glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));

        GLint first = 0;
        for (int j = 0; j < model->num_indices; j++) {
            if (model->materials[j].texture_id) glBindTexture(GL_TEXTURE_2D, model->materials[j].texture_id);
            glUniform1i(enable_texture, model->materials[j].texture_id);
            glUniform4fv(diffuse_color, 1, &model->materials[j].diffuse[0]);
            glDrawArrays(GL_TRIANGLES, first, model->indices[j]);
            first += model->indices[j];
        }

        glDisableVertexAttribArray(ATTRIB_POSITION);
        glDisableVertexAttribArray(ATTRIB_NORMAL);
        glDisableVertexAttribArray(ATTRIB_TEXCOORD);
    }
}
