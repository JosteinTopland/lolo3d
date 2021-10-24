#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "types.h"

void loadMTL(const char* filename);
Model *loadObj(const char *filename);

#endif