#include "shader.h"

//extern char* UNIFORMS[];
char* UNIFORMS[UN_LEN] = {
	"ModelMatrix",
	"ModelViewMatrix",
	"ModelViewProjectionMatrix",
	"CameraMatrix",
	"Bones",
	"NumBones",
	"Time",
	"Texture0",
	"Texture1",
	"Texture2",
	"Texture3",
	"PointLights",
	"PointLightsCount",
	"PointLightsColor"
};

void shader_init_uniform_index(shader_t* s) {
	int i;
	for (i = 0; i < UN_LEN; ++i) {
		s->uniform_index[i] = glGetUniformLocation(s->shader, UNIFORMS[i]);
}
}
