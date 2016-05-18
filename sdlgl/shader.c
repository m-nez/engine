#include "shader.h"

extern char* UNIFORMS[];

void shader_init_uniform_index(shader_t* s) {
	int i;
	for (i = 0; i < UN_LEN; ++i) {
		s->uniform_index[i] = glGetUniformLocation(s->shader, UNIFORMS[i]);
}
}
