#include "assets.h"
#include <stdlib.h>
#include <stdio.h>

assets_t* assets_new() {
	assets_t* assets = malloc(sizeof(assets_t));

	assets->shaders_map = hash_map_new(64);
	assets->shaders_data = malloc(sizeof(*assets->shaders_data) * 64);

	assets->models_map = hash_map_new(1024);
	assets->models_data = malloc(sizeof(*assets->models_data) * 1024);

	assets->textures_map = hash_map_new(1024);
	assets->textures_data = malloc(sizeof(*assets->textures_data) * 1024);

	assets->animations_map = hash_map_new(1024);
	assets->animations_data = malloc(sizeof(*assets->animations_data) * 1024);

	return assets;
}

static inline void strreplace(char* str, char a, char b) {
	while(*str != 0) {
		if (*str == a) {
			*str = b;
		}
		str++;
	}
}

void assets_load_ldl(assets_t* assets, const char* filename) {
	FILE* stream = fopen(filename, "r");
	int num;
	int i, j;
	char buff[4][FILENAME_MAX];
	char shaders[2][FILENAME_MAX];
	char* dirs[] = {
		"assets/shaders/",
		"assets/models/",
		"assets/textures/",
		"assets/animations/"
	};

	/* Shaders */
	fscanf(stream, "%d", &num);
	for(i = 0; i < num; ++i) {
		fscanf(stream, "%s %s", buff[0], buff[1]);
		for (j = 0; j < 2; ++j) {
			strcpy(shaders[j], dirs[0]);
			strcat(shaders[j], buff[j]);
		}
		strreplace(buff[0], '.', '\0');
		assets->shaders_data[assets->shaders_map->len].shader = load_shader(
				shaders[0], shaders[1]);
		shader_init_uniform_index(&assets->shaders_data[assets->shaders_map->len]);
		hash_map_set(
				assets->shaders_map, buff[0],
				assets->shaders_data + assets->shaders_map->len);
	}

	/* Models */
	fscanf(stream, "%d", &num);
	for(i = 0; i < num; ++i) {
		for(j = 0; j < 3; ++j) {
			fscanf(stream, "%s", buff[j]);
		}
		strcpy(buff[3], dirs[1]);
		strcat(buff[3], buff[1]);
		model_from_vbb(
				&assets->models_data[assets->models_map->len],
				buff[3],
				*(GLuint*)hash_map_get(assets->shaders_map, buff[2])
				);
		hash_map_set(
				assets->models_map, buff[0],
				assets->models_data + assets->models_map->len);
	}
	
	/* Textures */
	fscanf(stream, "%d", &num);
	for(i = 0; i < num; ++i) {
		fscanf(stream, "%s", buff[0]);
		strcpy(buff[1], dirs[2]);
		strcat(buff[1], buff[0]);

		assets->textures_data[assets->textures_map->len] = load_texture(buff[1]);
		strreplace(buff[0], '.', '\0');
		hash_map_set(
				assets->textures_map, buff[0],
				assets->textures_data + assets->textures_map->len);
	}

	/* Animations */
	fscanf(stream, "%d", &num);
	for(i = 0; i < num; ++i) {
		fscanf(stream, "%s", buff[0]);
		strcpy(buff[1], dirs[3]);
		strcat(buff[1], buff[0]);

		anim_from_file(
				assets->animations_data + assets->animations_map->len,
				buff[1]);
		strreplace(buff[0], '.', '\0');
		hash_map_set(
				assets->animations_map, buff[0],
				assets->animations_data + assets->animations_map->len);
	}


	fclose(stream);
}
