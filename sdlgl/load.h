#pragma once

#include "draw.h"
#include "data_types.h"

char* readfile(const char*);
GLuint load_shader(const char* vert, const char* frag);

/* Load from .vbb (Vertex Buffers + Bones) file */
unsigned int load_vbb(vbuff_t vbuff,
		const char* fname,
		GLuint shader,
		unsigned int* num_bones);
GLuint load_texture(const char* fname);
