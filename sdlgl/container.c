#include "container.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

char* UNIFORMS[UN_LEN] = {
	"ModelMatrix",
	"ModelViewMatrix",
	"ModelViewProjectionMatrix",
	"Bones",
	"NumBones",
};

static inline void get_uniforms_loc(GLuint shader, GLint* uniform_index) {
	int i;
	for (i = 0; i < UN_LEN; ++i) {
		uniform_index[i] = glGetUniformLocation(shader, UNIFORMS[i]);
		printf("%d", uniform_index[i]);
	}
}

char* SHADER_EXTENSIONS[]= {
	"vert",
	"frag"
};
#define SHADER_EXTENSIONS_LEN sizeof(SHADER_EXTENSIONS)/sizeof(*SHADER_EXTENSIONS)

int shader_map_get(shader_kv* shader_map, int len, const char* shader_name) {
	int i;
	for (i = 0; i < len; ++i) {
		if (strcmp(shader_name, shader_map[i].name) == 0)
			return i;
	}
	return -1;
}

int set_rs_shader(render_state_t* rs, shader_kv* shader_map, int len, const char* shader_name) {
	int index = shader_map_get(shader_map, len, shader_name);
	if (index == -1) 
		return -1;
	rs->shader = shader_map[index].shader;
	rs->uniform_index = shader_map[index].uniform_index;
	return 0;
}
void shader_map_add(shader_kv* shader_map, int* len, const char* fname) {
	int i;
	int d = -1, s = -1;
	for(i = 0; fname[i] != 0; ++i) {
		if (fname[i] == '/')
			s = i;
		if (fname[i] == '.')
			d = i;
	}
	if (d == -1 || d < s)
		d = i;

	int prefix_len = d - s - 1;
	char* shader_name = malloc(prefix_len + 1);
	strncpy(shader_name, fname + s + 1, prefix_len);
	shader_name[prefix_len] = 0;

	if (shader_map_get(shader_map, *len, shader_name) != -1) {
		free(shader_name);
		return;
	}

	char* shaders[SHADER_EXTENSIONS_LEN];
	for (i = 0; i < SHADER_EXTENSIONS_LEN; ++i) {
		int ext_len = strlen(SHADER_EXTENSIONS[i]);
		shaders[i] = malloc(sizeof(char) * (ext_len + d + 2));
		strncpy(shaders[i],  fname, d);
		shaders[i][d] = '.';
		strcpy(shaders[i] + d + 1,  SHADER_EXTENSIONS[i]);
		shaders[ext_len + d + 1] = 0;
	}

	int files_ok = 1;
	for(i = 0; i < SHADER_EXTENSIONS_LEN; ++i) {
		FILE* in = fopen(shaders[i], "r");
		if (in == NULL) {
			files_ok = 0;
			break;
		}
		fclose(in);
	}
	if (files_ok) {
		shader_map[*len].name = shader_name;
		shader_map[*len].shader = load_shader(shaders[0], shaders[1]);
		get_uniforms_loc(shader_map[*len].shader, shader_map[*len].uniform_index);
		++*len;
	}
	for (i = 0; i < SHADER_EXTENSIONS_LEN; ++i)
		free(shaders[i]);
}

void shader_map_add_folder(shader_kv* shader_map, int* len, const char* fname) {
	DIR* dir = opendir(fname);
	struct dirent* dent;
	char shader_name[FILENAME_MAX] = {0};
	int path_len = strlen(fname);
	strcpy(shader_name, fname);
	if (fname[path_len-1] != '/') {
		shader_name[path_len] = '/';
		path_len++;
	}


	while((dent = readdir(dir)) != NULL) {
		strcpy(shader_name + path_len, dent->d_name);
		shader_map_add(shader_map, len, shader_name);
	}
	free(dir);
}
