#include "sdlgl/sdlgl.h"
#include <math.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>

void h(int s) {
	printf("SIGINT received.\n");
	exit(1);
}

float stopper() {
	static clock_t last = 0;
	clock_t cur = clock();
	clock_t diff = cur - last;
	last = cur;
	return (float)diff/CLOCKS_PER_SEC;
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
	scene_init(&scene, settings);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	assets_load_ldl(assets, "assets/default.ldl");
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

	gobjects_t* gobjects = gobjects_new(1024, render_states);

	gobject_t* d1 = gobjects_add_draw_phys(gobjects, "box", "box", COL_BOX);
	gobject_t* d2 = gobjects_add_draw_phys(gobjects, "basketball", "basketball", COL_SPHERE);
	gobject_t* d3 = gobjects_add_draw_phys(gobjects, "table", "table", COL_PLANE);
	gobject_t* d4 = gobjects_add_draw_phys(gobjects, "t2", "table", COL_PLANE);
	gobject_t* d5 = gobjects_add_draw_phys(gobjects, "t3", "table", COL_PLANE);

	gobject_t* cam = gobjects_add(gobjects, "camera");
	gobject_attach_mat(cam, scene.camera);

	vec3 grav;
	vec3set(grav, 0,0,-9.81);

	d1->col_object->physics_type = PHYSICS_TYPE_RIGID;
	d1->col_object->restitution = 0.2;
	vec3set(((col_shape_box_t*)d1->col_object)->dimensions, 2, 2, 8);
	col_shape_box_calc((col_shape_box_t*)d1->col_object);
	d2->col_object->physics_type = PHYSICS_TYPE_RIGID;
	col_shape_sphere_calc((col_shape_sphere_t*)d2->col_object);

mat4 r, g;

vmathM4MakeRotationY((VmathMatrix4*)r, M_PI/2.0);
mat4mul(g, r, d4->col_object->transform);
mat4cpy(d4->col_object->transform, g);

vmathM4MakeRotationX((VmathMatrix4*)r, M_PI/2.0);
mat4mul(g, r, cam->transform);
mat4cpy(cam->transform, g);

vmathM4MakeRotationY((VmathMatrix4*)r, -M_PI/2.0);
mat4mul(g, r, d5->col_object->transform);
mat4cpy(d5->col_object->transform, g);

	gobject_move_xyz(cam, 6.0, -10.0, -1);
	gobject_move_xyz(d1, 3, 0.0, 2.0);
	gobject_move_xyz(d2, 8, 0.0, -4.0);
	gobject_move_xyz(d3, 8.0, 0.0, -5.0);
	gobject_move_xyz(d4, 0.0, 0.0, 3.0);
	gobject_move_xyz(d5, 16.0, 0.0, 3.0);

	vec3set(d1->col_object->angular_velocity, 0, -2, 0);
	vec3set(d2->col_object->velocity, -8, 0, 0);

	float V = 9;
	scene.point_lights.len = 2;
	vec3set(scene.point_lights.col[0], V,V,V);
	V = 9;
	vec3set(scene.point_lights.col[1], V,V,V);
	vec3set(scene.point_lights.pos[1] + 12, 6, -10, -1);

	for (int i = 0; i < 400; ++i) {
		float dt = 0.016;
		scene.point_lights.pos[0][12] = 4 + 2*cosf(scene.time);
		scene.point_lights.pos[0][13] = 0 + 2*sinf(scene.time);

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
	//	usleep(dt * 1000000);
	}

	return 0;
}
