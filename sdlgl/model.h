#pragma once

#include "draw.h"
#include "load.h"

typedef struct {
	vbuff_t vbuff;
	unsigned int num_elems;
	unsigned int num_bones;
} model_t;

void model_from_vbb(model_t* model, const char* filename, GLuint shader);
