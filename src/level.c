#include "level.h"

#include <stdio.h>
#include <cglm/cglm.h>

#include "globals.h"
#include "obj_loader.h"

void load_level() {
    Model *rock = load_obj("assets/rock.obj");
    Model *tree = load_obj("assets/tree.obj");
    Model *tux = load_obj("assets/hero.obj");

    Model *models[] = {
        rock,
        tree,
        tux
    };

    int width = 11;
    int height = 11;
    float space = 2.5f;
    int map[] = {
        1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 2,
        1, 2, 2, 1, 0, 0, 0, 1, 1, 2, 2,
        0, 2, 2, 1, 1, 1, 0, 1, 1, 1, 2,
        0, 0, 2, 2, 1, 1, 0, 1, 1, 1, 2,
        0, 0, 0, 0, 1, 1, 0, 1, 1, 2, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 2, 2, 2, 0, 0, 0, 2, 2, 0, 0,
        2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0,
        1, 2, 2, 1, 0, 0, 0, 0, 2, 2, 0,
        1, 1, 1, 1, 1, 1, 0, 3, 0, 0, 0
    };

    level = malloc(sizeof(Level));
    level->num_objects = width * height;
    level->objects = malloc(sizeof(Object) * level->num_objects);
    for (int i = 0; i < level->num_objects; i++) {
        if (map[i]) {
            level->objects[i].x = (i % width) * space;
            level->objects[i].y = (height - i / width) * space;
            level->objects[i].rotation = 0;
            level->objects[i].model = models[map[i] - 1];
        } else {
            level->objects[i].model = NULL;
        }
    }

    glm_perspective(glm_rad(50.0f), (float)window_width / window_height, 0.1f, 200.0f, proj_mat);
    glUniformMatrix4fv(proj_mat_id, 1, GL_FALSE, &proj_mat[0][0]);

    vec3 eye = { 20.0f, -5.0f, 5.0f };
    vec3 center = { width * space * 0.42f, height * space * 0.5f, 0.0f };
    glm_lookat(eye, center, GLM_ZUP, view_mat);
    glUniformMatrix4fv(view_mat_id, 1, GL_FALSE, &view_mat[0][0]);
}
