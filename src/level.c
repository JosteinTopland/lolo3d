#include "level.h"

#include <stdio.h>
#include <cglm/cglm.h>

#include "globals.h"
#include "obj_loader.h"

void loadLevel() {
    Model *rock = loadObj("assets/rock.obj");
    Model *tree = loadObj("assets/lowpolytree.obj");

    Model *models[] = {
        rock,
        tree
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
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
    };

    level = malloc(sizeof(Level));
    level->numObjects = width * height;
    level->objects = malloc(sizeof(Object) * level->numObjects);
    for (int i = 0; i < level->numObjects; i++) {
        if (map[i]) {
            level->objects[i].x = (i % width) * space;
            level->objects[i].y = (height - i / width) * space;
            level->objects[i].rotation = 0;
            level->objects[i].model = models[map[i] - 1];
        } else {
            level->objects[i].model = NULL;
        }
    }

    glm_perspective(glm_rad(50.0f), (float)windowWidth / windowHeight, 0.1f, 200.0f, projMat);
    glUniformMatrix4fv(projMatId, 1, GL_FALSE, &projMat[0][0]);

    vec3 eye = { 30.0f, -10.0f, 15.0f };
    vec3 center = { width * space * 0.42f, height * space * 0.5f, 0.0f };
    glm_lookat(eye, center, GLM_ZUP, viewMat);
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &viewMat[0][0]);
}
