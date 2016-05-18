#pragma once
#include "hash_map.h"
#include "load.h"
#include "model.h"
#include "anim.h"
#include "shader.h"

typedef struct {
	hash_map_t* shaders_map;
	hash_map_t* models_map;
	hash_map_t* textures_map;
	hash_map_t* animations_map;

	shader_t* shaders_data;
	model_t* models_data;
	GLuint* textures_data;
	anim_t* animations_data;
} assets_t;

assets_t* assets_new();
void assets_load_ldl(assets_t* assets, const char* filename);
