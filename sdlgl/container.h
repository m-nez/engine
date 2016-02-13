#pragma once
#include "load.h"
#include "draw.h"

/* shader key value association */
#define MAX_SHADERS 32
typedef struct {
	char* name;
	GLuint shader;
	GLint uniform_index[UN_LEN];
} shader_kv;

typedef struct {
	GLuint index;
	GLint uniform_index[UN_LEN];
} shader_val;

typedef struct {
	char* keys[MAX_SHADERS];
	shader_val vals[MAX_SHADERS];
	int len;
} shader_map_t;

/* Returns shader index in the map if found, if not return -1 */
int shader_map_get(shader_kv* shader_map, int len, const char* shader_name);
void shader_map_add(shader_kv* shader_map, int* len, const char* fname);
void shader_map_add_folder(shader_kv* shader_map, int* len, const char* fname);

/* Returns 0 if shader was set otherwise returns -1 */
int set_rs_shader(render_state_t* rs, shader_kv* shader_map, int len, const char* shader_name);
