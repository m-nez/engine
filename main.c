#include "sdlgl/sdlgl.h"
#include "sdlgl/debug.h"

#include <unistd.h>

int main(int argc, char** argv) {
	SDLGL_Settings* settings = SDLGL_SettingsDefault();
	SDL_Window* window;
	SDL_GLContext* context;
	SDLGL_Init(settings, &window, context);

	printf("%p\n", window);
	//TESTING
	vbuff_t vbuff[1];
	gen_vbuff(vbuff[0]);
	render_state_t r_states[1];
	dobject_t dobs[1];
	r_states[0].vao = vbuff[0][VBF_VAO];

	shader_kv s_map[16];
	int s_map_len = 0;
	shader_map_add_folder(s_map, &s_map_len, "assets/shaders");

	set_rs_shader(&r_states[0], s_map, s_map_len, "test");
	r_states[0].len = 1;
	r_states[0].num_elem = load_vbb(vbuff[0], "assets/circle.vbb", r_states[0].shader);
	r_states[0].num_tex = 0;

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_dobjects(dobs, r_states, 1);
	SDL_GL_SwapWindow(window);
	usleep(1000000);
	return 0;
}
