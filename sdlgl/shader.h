#pragma once

#include "draw.h"

typedef struct {
	GLuint shader;
	GLint uniform_index[UN_LEN];
} shader_t;

void shader_init_uniform_index(shader_t* s);
