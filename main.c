#include "sdlgl/sdlgl.h"
#include "sdlgl/debug.h"
#include <math.h>
#include <unistd.h>

#include <signal.h>

void h(int s) {
	printf("SIGINT received.\n");
	exit(1);
}

int main(int argc, char** argv) {
	signal(SIGINT, h);
	SDLGL_Settings* settings = SDLGL_SettingsDefault();
	SDL_Window* window;
	SDL_GLContext* context = 0;
	SDLGL_Init(settings, &window, context);

	assets_t* assets = assets_new();

	//TESTING
	scene_properties_t scene;
	mat4identity(scene.camera);
	mat4projection(
			scene.projection,
			settings->width, settings->height,
			1.0, 1000.0);
	scene.time = 0;
	scene.frame_buffer = frame_buffer_new(settings->width, settings->height);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	assets_load_ldl(assets, "assets/default.ldl");

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

	gobject_t* d1 = gobjects_add_draw_phys(gobjects, "l1", "box", COL_BOX);
	gobject_t* d2 = gobjects_add_draw_phys(gobjects, "l2", "basketball", COL_SPHERE);
	gobject_t* d3 = gobjects_add_draw_phys(gobjects, "table", "table", COL_PLANE);
	gobject_t* d4 = gobjects_add_draw_phys(gobjects, "t2", "table", COL_PLANE);

	gobject_t* cam = gobjects_add(gobjects, "camera");
	gobject_attach_mat(cam, scene.camera);

	vec3 grav;
	vec3set(grav, 0,0,-9.81);

	d1->col_object->physics_type = PHYSICS_TYPE_RIGID;
	d2->col_object->physics_type = PHYSICS_TYPE_RIGID;
	vec3set(((col_shape_box_t*)d1->col_object)->dimensions, 2, 2, 2);

mat4 r, g;

vmathM4MakeRotationY(r, M_PI/2.0);
mat4mul(r, d4->col_object->transform, g);
mat4cpy(d4->col_object->transform, g);

vmathM4MakeRotationX(r, M_PI/2.0);
mat4mul(r, cam->transform, g);
mat4cpy(cam->transform, g);

	gobject_move_xyz(cam, 6.0, -10.0, -1);
	gobject_move_xyz(d1, 2.0, 0.0, 3);
	gobject_move_xyz(d2, 2.5, 0.0, 8.0);
	gobject_move_xyz(d3, 8.0, 0.0, -5.0);
	gobject_move_xyz(d4, 0.0, 0.0, 3.0);

	for (int i = 0; i < 600; ++i) {
		if (i > 60 && i % 10 == 0) {
			char name[16];
			sprintf(name, "bb%d", i);
			d1 = gobjects_add_draw_phys(gobjects, name, "basketball", COL_SPHERE);
			d1->col_object->physics_type = PHYSICS_TYPE_RIGID;
			gobject_move_xyz(d1, 2, 0, 6);
			vec3set(d1->col_object->velocity, 1, (i / 15 % 5) - 2, -1);
		}
		float dt = 0.016;
		col_world_apply_acceleration(gobjects->col_world, grav, dt);
		col_world_resolve_col(gobjects->col_world);
		col_world_apply_velocity(gobjects->col_world, dt);
		gobjects_apply(gobjects, dt);

		glEnable(GL_DEPTH_TEST);
		scene_bind_frame_buffer(&scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_render_states(render_states->data, render_states->map->len, &scene);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		draw_render_states(render_states_post->data, render_states_post->map->len, &scene);
		SDL_GL_SwapWindow(window);
		scene.time += dt;
		usleep(dt * 1000000);
	}

	return 0;
}
