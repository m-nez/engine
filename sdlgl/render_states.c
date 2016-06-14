#include "render_states.h"

#include <stdlib.h>
#include <stdio.h>

static void render_state_from_model_shader_textures(
		render_state_t* rs,
		model_t* model,
		shader_t* shader,
		GLuint* textures,
		int num_tex,
		int len
		) {
	/* Make sure everything is initialized */
	int i;

	rs->vao = model->vbuff[VBF_VAO];
	rs->num_bones = model->num_bones;
	rs->shader = shader->shader;
	for(i = 0; i < num_tex; ++i) {
		rs->textures[i] = textures[i];
	}
	rs->num_elem = model->num_elems;
	rs->num_tex = num_tex;
	rs->len = len;
	rs->uniform_index = shader->uniform_index;

	rs->dobjects_size = len + 1; /* Size at least 1 */
	rs->dobjects = malloc(sizeof(dobject_t) * rs->dobjects_size);
	for(i = 0; i < rs->dobjects_size; ++i) {
		dobject_init(rs->dobjects + i, rs);
	}
}

render_states_t* render_states_new() {
	render_states_t* rs = malloc(sizeof(render_states_t));
	rs->map = hash_map_new(RS_MAX_RENDER_STATES);
	rs->data = malloc(sizeof(render_state_t) * RS_MAX_RENDER_STATES);
	return rs;
}
void render_states_add(
		render_states_t* r,
		model_t* model,
		shader_t* shader,
		GLuint* textures,
		int num_tex,
		int len,
		const char* key) {
		
		render_state_from_model_shader_textures(
				r->data + r->map->len,
				model,
				shader,
				textures,
				num_tex,
				len
				);

		hash_map_set(
				r->map, key,
				r->data + r->map->len);
}

void render_states_load_RSB(render_states_t* rs, assets_t* assets, const char* filename) {
	int i, j;
	model_t* model;
	shader_t* shader;
	GLuint textures[MAX_TEXTURES];
	int n, len;
	int num_textures;
	char buf[FILENAME_MAX];
	char key[FILENAME_MAX];
	FILE* stream = fopen(filename, "r");
	if (stream == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", filename);
		return;
	}

	fscanf(stream, "%d", &n);

	for(i = 0; i < n; ++i) {
		fscanf(stream, " %[^\n]", key);
		fscanf(stream, " %[^\n]", buf);
		model = hash_map_get(assets->models_map, buf);
		if (model == NULL) {
			fprintf(stderr, "Error: No model named %s\n", buf);
		}
		fscanf(stream, " %[^\n]", buf);
		shader = hash_map_get(assets->shaders_map, buf);
		if (shader == NULL) {
			fprintf(stderr, "Error: No shader named %s\n", buf);
		}
		fscanf(stream, "%d", &num_textures);
		for(j = 0; j < num_textures; ++j) {
			GLuint* tp;
			fscanf(stream, " %[^\n]", buf);
			tp = hash_map_get(assets->textures_map, buf);
			if (tp == NULL) {
				fprintf(stderr, "Error: No texture named %s\n", buf);
			}
			textures[j] = *tp;
		}
		fscanf(stream, "%d", &len);
		render_states_add(rs, model, shader, textures, num_textures, len, key);
	}
	fclose(stream);
}
