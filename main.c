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
	assets_load_ldl(assets, "assets/loadlist.ldl");

	//TESTING
	scene_properties_t scene;
	mat4identity(scene.camera);
	scene.camera[14] = 18.0;
	mat4projection(
			scene.projection,
			settings->width, settings->height,
			1.0, 1000.0);
	scene.time = 0;
	scene.frame_buffer = frame_buffer_new(settings->width, settings->height);

	shader_map_t* s_map = shader_map_new();
	shader_map_add_folder(s_map, "assets/shaders");

	rs_vec_t* rs = rs_vec_new();
	rs_vec_t* rs_post = rs_vec_new();
	char* tex[] = {"assets/textures/leech_diffuse_texture.png"};

	rs_vec_add(rs, s_map, "test", "assets/models/leech.vbb", 1, tex, 1);
	rs_vec_realloc_bones(rs);


	rs_vec_add(rs_post, s_map, "post_none", "assets/models/quad.vbb", 0, 0, 1);

	rs_post->rs[0].textures[0] = scene.frame_buffer->tex_color_buffer;
	rs_post->rs[0].textures[1] = scene.frame_buffer->tex_depth_buffer;
	rs_post->rs[0].num_tex = 2;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	anim_t* anim = (anim_t*)hash_map_get(assets->animations_map, "leech_roll");

	GLuint t[2] = {
		*(GLuint*) hash_map_get(assets->textures_map, "leech_diffuse_texture"),
		*(GLuint*) hash_map_get(assets->textures_map, "leech_diffuse_texture_bloodied")
	};

	for (int i = 0; i < 60 * 5; ++i) {
		//rs->rs[0].textures[0] = t[(int)(i / 60.0) % 2];
		anim_eval(anim, scene.time, rs->dobjects[0].bones);
		glEnable(GL_DEPTH_TEST);
		scene_bind_frame_buffer(&scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_dobjects(rs->dobjects, rs->rs, rs->num_rs, &scene);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		draw_dobjects(rs_post->dobjects, rs_post->rs, rs_post->num_rs, &scene);
		SDL_GL_SwapWindow(window);
		scene.time += 0.016;
		usleep(16000);
	}

	shader_map_delete(s_map);
	rs_vec_delete(rs);
	rs_vec_delete(rs_post);

	return 0;
}
