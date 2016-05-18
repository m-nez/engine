#pragma once
#include "load.h"
#include "draw.h"

#define MAX_SHADERS 32
#define MAX_RENDER_STATES 1024
#define MAX_DOBJECTS 2048
/* shader key value association */
typedef struct {
	GLuint shader;
	GLint uniform_index[UN_LEN];
} shader_val;

typedef struct {
	char* keys[MAX_SHADERS];
	shader_val vals[MAX_SHADERS];
	int len;
} shader_map_t;

void shader_map_add_folder(shader_map_t* shader_map, const char* fname);
void shader_map_add(shader_map_t* shader_map, const char* fname);
shader_val* shader_map_get(shader_map_t* shader_map, const char* shader_name);
shader_map_t* shader_map_new();
void shader_map_delete(shader_map_t* shader_map);

/* Returns 0 if shader was set otherwise returns -1 */
int set_rs_shader(
		render_state_t* rs,
		shader_map_t* shader_map,
	   	const char* shader_name);


typedef struct {
	render_state_t* rs;
	dobject_t* dobjects;
	vbuff_t* vbuffs;
	int* index;	/* index[i] == index of the first dobject with i'th render_state */
	int num_rs;
} rs_vec_t;

rs_vec_t* rs_vec_new();
void rs_vec_delete(rs_vec_t* rs);
/* Returns number of objects loaded, -1 on failure */
int load_rs_vec(
		rs_vec_t* rs,
		shader_map_t* shader_map,
		const char* fname,
		vbuff_t* vbuffs);

void rs_vec_add(
		rs_vec_t* rs,
		shader_map_t* shader_map,
		const char* shader, const char* vbb,
		int num_tex, char** textures, int len);

void rs_vec_realloc_bones(rs_vec_t* rs);
