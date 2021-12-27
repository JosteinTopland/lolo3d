#include "level.h"

#include <stdio.h>
#include <cglm/cglm.h>

#include "globals.h"
#include "obj_loader.h"

void loadLevel() {
    Model *tree = loadObj("assets/lowpolytree.obj");
    Model *cookie = loadObj("assets/11571_Gingerbread_cookie_male_V2_l2.obj");
    Model *test = loadObj("assets/test.obj");
    Model *box = loadObj("assets/box.obj");

    Model *models[] = {
        tree,
        box,
        cookie,
        test
    };

    int map[] = {
        4, 0, 0, 3,
        0, 0, 0, 0,
        0, 0, 0, 0,
        2, 0, 0, 1
    };

    level = malloc(sizeof(Level));
    level->numObjects = 4 * 4;
    level->objects = malloc(sizeof(Object) * level->numObjects);
    for (int i = 0; i < level->numObjects; i++) {
        if (map[i]) {
            level->objects[i].x = (i % 4) * 2.0f;
            level->objects[i].y = (i / 4) * 2.0f;
            level->objects[i].rotation = 0;
            level->objects[i].model = models[map[i] - 1];
        } else {
            level->objects[i].model = NULL;
        }
    }

    glm_perspective(30.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f, projMat);
    glUniformMatrix4fv(projMatId, 1, GL_FALSE, &projMat[0][0]);

    vec3 eye = { 8.0f, 3.0f, 10.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, -1.0f, 0.0f };
    glm_lookat(eye, center, up, viewMat);
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &viewMat[0][0]);
}