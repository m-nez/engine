#include "sdlgl/sdlgl.h"
#include "sdlgl/debug.h"

#include <math.h>
#include <unistd.h>

int main(int argc, char** argv) {
	SDLGL_Settings* settings = SDLGL_SettingsDefault();
	SDL_Window* window;
	SDL_GLContext* context;
	SDLGL_Init(settings, &window, context);

	assets_t* assets = assets_new();
	//assets_load_ldl(assets, "assets/loadlist.ldl");
	assets_load_ldl(assets, "assets/MTGcards.ldl");

	//TESTING
	scene_properties_t scene;
	mat4identity(scene.camera);
	scene.camera[14] = 4.0;
	mat4projection(
			scene.projection,
			settings->width, settings->height,
			1.0, 1000.0);
	scene.time = 0;
	scene.frame_buffer = frame_buffer_new(settings->width, settings->height);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	anim_t* anim = (anim_t*)hash_map_get(assets->animations_map, "leech_roll");

	/* Render States */
	render_states_t* render_states = render_states_new();
	
	//render_states_load_RSB(render_states, assets, "assets/rs_builder.rsb");
	render_states_load_RSB(render_states, assets, "assets/MTGcards.rsb");

	render_states->data[0].dobjects[0].model_mat[14] -= 0.2;
	//render_states->data[0].dobjects[0].model_mat[12] += 12.2;

	//render_state_remove_at(&render_states->data[0], 0);
	for(int i = 0; i < render_states->map->len; ++i) {
		for(int j = 0; j < render_states->data[i].len; ++j) {
			render_states->data[i].dobjects[j].model_mat[12] += 0.7 * i;
			render_states->data[i].dobjects[j].model_mat[13] -= 0.2 * j;
		}
	}
	
	GLuint text[8];

	render_states_t* render_states_post = render_states_new();
	text[0] = scene.frame_buffer->tex_color_buffer;
	text[1] = scene.frame_buffer->tex_depth_buffer;
	render_states_add(
			render_states_post,
			hash_map_get(assets->models_map, "quad"),
			hash_map_get(assets->shaders_map, "post_none"),
			text,
			2,
			1,
			"quad");

	for (int i = 0; i < 1; ++i) {
		//anim_eval(anim, scene.time, render_states->dobjects[0].bones);
		glEnable(GL_DEPTH_TEST);
		scene_bind_frame_buffer(&scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw_dobjects(render_states->dobjects, render_states->data, render_states->map->len, &scene);
		draw_render_states(render_states->data, render_states->map->len, &scene);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		//draw_dobjects(render_states_post->dobjects, render_states_post->data, render_states_post->map->len, &scene);
		draw_render_states(render_states_post->data, render_states_post->map->len, &scene);
		SDL_GL_SwapWindow(window);
		scene.time += 0.016;
		usleep(300*16000);
	}

	return 0;
}
