add_library(sdlgl
	anim.c
	assets.c
	data_types.c
	debug.c
	draw.c
	gobject.c
	gobject.h
	gobjects.c
	gobjects.h
	hash_map.c
	load.c
	model.c
	render_state.c
	render_states.c
	shader.c
	window.c
	)

set(LIBS m SDL2 SDL2_image GLEW GLU GL)

target_link_libraries(sdlgl ${LIBS})

target_include_directories(sdlgl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
