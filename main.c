#include "sdlgl/sdlgl.h"
#include "sdlgl/debug.h"
#include <math.h>
#include <unistd.h>

#include <signal.h>

void h(int s) {
	printf("SIGINT: Interupting\n");
	exit(1);
}

int main(int argc, char** argv) {
	signal(SIGINT, h);

	SDLGL_Settings* settings = SDLGL_SettingsDefault();
	SDL_Window* window;
	SDL_GLContext* context;
	SDLGL_Init(settings, &window, context);

	assets_t* assets = assets_new();

	//TESTING
	scene_properties_t scene;
	mat4identity(scene.camera);
	scene.camera[14] = 20.0;
	scene.camera[13] = -2.0;
	mat4projection(
			scene.projection,
			settings->width, settings->height,
			1.0, 1000.0);
	scene.time = 0;
	scene.frame_buffer = frame_buffer_new(settings->width, settings->height);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	assets_load_ldl(assets, "assets/default.ldl");
	anim_t* anim = (anim_t*)hash_map_get(assets->animations_map, "leech_roll");

	/* Render States */
	render_states_t* render_states = render_states_new();

	
	render_states_load_RSB(render_states, assets, "assets/default.rsb");
	
	GLuint text[8];

	render_states_t* render_states_post = render_states_new();
	text[0] = scene.frame_buffer->tex_color_buffer;
	text[1] = scene.frame_buffer->tex_depth_buffer;
	render_states_add(
			render_states_post,
			hash_map_get(assets->models_map, "quad:post_none"),
			hash_map_get(assets->shaders_map, "post_none"),
			text,
			2,
			1,
			"quad");

	gobjects_t* gobjects = gobjects_new(256, render_states);
	gobject_t* d = gobjects_add_drawable(gobjects, "l1", "car3");
	gobject_t* d2 = gobjects_add_drawable(gobjects, "l2", "driver");
	gobject_t* cam = gobjects_add(gobjects, "camera");
	gobject_attach_mat(cam, scene.camera);
	d->transform[12] += 7;

	hash_map_print_keys(render_states->map);
	render_state_t* leech_rs = hash_map_get(render_states->map, "driver");

	gobject_move_xyz(cam, 2.0, 0.0, 16.0);

	gobject_set_anim(d2, anim, 0.5);

	anim = (anim_t*)hash_map_get(assets->animations_map, "car_front_flip");
	gobject_set_anim(d, anim, 0.0);

	for (int i = 0; i < 300; ++i) {
		gobjects_apply(gobjects, 0.016);

		glEnable(GL_DEPTH_TEST);
		scene_bind_frame_buffer(&scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_render_states(render_states->data, render_states->map->len, &scene);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		draw_render_states(render_states_post->data, render_states_post->map->len, &scene);
		SDL_GL_SwapWindow(window);
		scene.time += 0.016;
		usleep(16000);
	}

	return 0;
}
