#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "types.h"

void load_mtl(const char* filename, Model* model);
Model *load_obj(const char *filename);
void free_model(Model* model);

#endif
