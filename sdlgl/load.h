#pragma once

#include "draw.h"
#include "data_types.h"

char* readfile(const char*);
GLuint load_shader(const char* vert, const char* frag);

/* Load from .vbb (Vertex Buffers + Bones) file 
 * Return number of elements */
unsigned int load_vbb(vbuff_t vbuff, const char* fname, GLuint shader);
