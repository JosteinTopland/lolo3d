#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "types.h"

void loadMTL(const char* filename, Model* model);
Model *loadObj(const char *filename);
void freeModel(Model* model);

#endif
