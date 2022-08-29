#include "level.h"

#include "obj_loader.h"

Level *load_level(int num) {
    Level *level = malloc(sizeof(Level));

    level->music = Mix_LoadMUS("assets/ARCANE.MOD");
    Mix_VolumeMusic(50);
    Mix_PlayMusic(level->music, -1);

    level->num_models = 3;
    level->models = malloc(sizeof(Model) * level->num_models);
    level->models[0] = load_obj("assets/grass.obj");
    level->models[1] = load_obj("assets/rock.obj");
    level->models[2] = load_obj("assets/cup.obj");

    level->num_objects = width * height;
    level->objects = malloc(sizeof(Object) * level->num_objects);
    for (int i = 0; i < level->num_objects; i++) {
        int offset = num * width * height;
        char c = levels[i + offset];
        Model *model = level->models[c - 1];
        if (c) {
            level->objects[i].x = (i % width) * 2;
            level->objects[i].y = (height - i / width) * 2;
            level->objects[i].rotation = 0;
            level->objects[i].model = model;
        } else {
            level->objects[i].model = NULL;
        }
    }

    return level;
}

void free_level(Level *level)
{
    for (int i = 0; i < level->num_models; i++) {
        free_model(level->models[i]);
    }
    Mix_FreeMusic(level->music);
    free(level);
}
