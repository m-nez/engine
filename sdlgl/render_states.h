#pragma once

#include "draw.h"
#include "hash_map.h"
#include "model.h"
#include "shader.h"
#include "assets.h"

#define RS_MAX_DOBJECTS 2048
#define RS_MAX_RENDER_STATES 1024

typedef struct {
	hash_map_t* map;
	render_state_t* data;
	dobject_t* dobjects;
	int* index;
} render_states_t;

render_states_t* render_states_new();
void render_states_add(
		render_states_t* r,
		model_t* model,
		shader_t* shader,
	   	GLuint* textures,
	   	int num_tex,
		int len,
		const char* key);

void render_states_realloc_bones(
		render_states_t* rs);

void render_states_load_RSB(render_states_t* rs, assets_t* assets, const char* filename);
