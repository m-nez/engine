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
	"Time",
	"Texture0",
	"Texture1",
	"Texture2",
	"Texture3"
};

static inline void get_uniforms_loc(GLuint shader, GLint* uniform_index) {
	int i;
	for (i = 0; i < UN_LEN; ++i) {
		uniform_index[i] = glGetUniformLocation(shader, UNIFORMS[i]);
	}
}

char* SHADER_EXTENSIONS[]= {
	"vert",
	"frag"
};
#define SHADER_EXTENSIONS_LEN sizeof(SHADER_EXTENSIONS)/sizeof(*SHADER_EXTENSIONS)

shader_val* shader_map_get(shader_map_t* shader_map, const char* shader_name) {
	int i;
	for (i = 0; i < shader_map->len; ++i) {
		if (strcmp(shader_name, shader_map->keys[i]) == 0)
			return &shader_map->vals[i];
	}
	return NULL;
}

int set_rs_shader(render_state_t* rs, shader_map_t* shader_map, const char* shader_name) {
	shader_val* val = shader_map_get(shader_map, shader_name);
	if (val == NULL) 
		return -1;
	rs->shader = val->shader;
	rs->uniform_index = val->uniform_index;
	return 0;
}

void shader_map_add(shader_map_t* shader_map, const char* fname) {
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

	if (shader_map_get(shader_map, shader_name) != NULL) {
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
		shader_map->keys[shader_map->len] = shader_name;
		shader_map->vals[shader_map->len].shader = load_shader(shaders[0], shaders[1]);
		get_uniforms_loc(	shader_map->vals[shader_map->len].shader,
							shader_map->vals[shader_map->len].uniform_index);
		++shader_map->len;
	}
	for (i = 0; i < SHADER_EXTENSIONS_LEN; ++i)
		free(shaders[i]);
}

void shader_map_add_folder(shader_map_t* shader_map, const char* fname) {
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
		shader_map_add(shader_map, shader_name);
	}
	free(dir);
}

shader_map_t* shader_map_new() {
	shader_map_t* shader_map = malloc(sizeof(shader_map_t));
	shader_map->len = 0;
	return shader_map;
}

void shader_map_delete(shader_map_t* shader_map) {
	int i;
	for (i = 0; i < shader_map->len; ++i) {
		free(shader_map->keys[i]);
	}
	free(shader_map);
}

rs_vec_t* rs_vec_new() {
	int i;
	rs_vec_t* rs = malloc(sizeof(rs_vec_t));
	rs->rs = malloc(MAX_RENDER_STATES * sizeof(render_state_t));
	rs->vbuffs = malloc(MAX_RENDER_STATES * sizeof(vbuff_t));
	rs->index = malloc(MAX_RENDER_STATES * sizeof(int));
	rs->dobjects = malloc(MAX_DOBJECTS * sizeof(dobject_t));
	for (i = 0; i < MAX_DOBJECTS; ++i) {
		mat4identity(rs->dobjects[i].model_mat);
		rs->dobjects[i].bones = 0;
	}

	rs->index[0] = 0;
	rs->num_rs = 0;
	return rs;
}

void rs_vec_delete(rs_vec_t* rs) {
	int i;

	free(rs->rs);
	for(i = 0; i < MAX_DOBJECTS; ++i) {
		free(rs->dobjects[i].bones);
	}
	free(rs->dobjects);
	free(rs->vbuffs);
	free(rs->index);
	free(rs);
}

void rs_vec_add(rs_vec_t* rs, shader_map_t* shader_map, const char* shader, const char* vbb, int num_tex, char** textures, int len) {
	shader_val* s_val = shader_map_get(shader_map, shader);
	gen_vbuff(rs->vbuffs[rs->num_rs]);
	rs->rs[rs->num_rs].num_elem = load_vbb(rs->vbuffs[rs->num_rs],
			vbb, s_val->shader,
			&rs->rs[rs->num_rs].num_bones);
	rs->rs[rs->num_rs].vao = rs->vbuffs[rs->num_rs][VBF_VAO];
	rs->rs[rs->num_rs].uniform_index = s_val->uniform_index;
	rs->rs[rs->num_rs].shader = s_val->shader;
	rs->rs[rs->num_rs].num_tex = num_tex;
	int i;
	for (i = 0; i < num_tex; ++i) {
		rs->rs[rs->num_rs].textures[i] = load_texture(textures[i]);
	}
	rs->rs[rs->num_rs].len = len;
	rs->index[rs->num_rs + 1] = rs->index[rs->num_rs] + len; 
	rs->num_rs++;
}

int rs_vec_load(rs_vec_t* rs, shader_map_t* shader_map, const char* fname, vbuff_t* vbuffs) {
	FILE* in = fopen(fname, "rb");
	char tmp_fname[FILENAME_MAX];
	int i, j;
	fread(&rs->num_rs, sizeof(int), 1, in);
	for(i = 0; i < rs->num_rs; ++i) {
		fscanf(in, "%s", tmp_fname);
		shader_val* s_val = shader_map_get(shader_map, tmp_fname);
		if (s_val == NULL) {
			fprintf(stderr, "Error: No shader named %s\n", tmp_fname);
			return -1;
		}
		fread(&rs->rs[i].num_tex, sizeof(int), 1, in);
		for(j = 0; j < rs->rs[i].num_tex; ++j) {
			fgets(tmp_fname, FILENAME_MAX, in);
			rs->rs[i].textures[j] = load_texture(tmp_fname);
		}
		fgets(tmp_fname, FILENAME_MAX, in);
		rs->rs[i].num_elem = load_vbb(
				vbuffs[i],
			   	tmp_fname,
				s_val->shader,
				&rs->rs[i].num_bones
				);
		rs->rs[i].vao = vbuffs[i][VBF_VAO];
		fread(&rs->rs[i].len, sizeof(int), 1, in);
	}
	return 0;
}

void rs_vec_realloc_bones(rs_vec_t* rs) {
	int i, j, k;
	for (i = 0; i < rs->num_rs; ++i) {
		for (j = rs->index[i]; j < rs->index[i+1]; ++j) {
			rs->dobjects[j].bones = realloc(
					rs->dobjects[j].bones,
					sizeof(*(rs->dobjects[j].bones)) * rs->rs[i].num_bones);
			for(k = 0; k < rs->rs[i].num_bones; ++k) {
				mat4identity(rs->dobjects[j].bones[k]);
			}
		}
	}
}
