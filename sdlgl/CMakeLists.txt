add_library(sdlgl
	anim.c
	anim.h
	assets.c
	assets.h
	data_types.c
	data_types.h
	debug.c
	debug.h
	draw.c
	draw.h
	gobject.c
	gobject.h
	gobjects.c
	gobjects.h
	hash_map.c
	hash_map.h
	load.c
	load.h
	model.c
	model.h
	render_state.c
	render_state.h
	render_states.c
	render_states.h
	shader.c
	shader.h
	window.c
	window.h
	nf_set.c
	nf_set.h
	)

add_subdirectory(physics)

set(LIBS m SDL2 SDL2_image GLEW GLU GL physics)

target_link_libraries(sdlgl ${LIBS})

target_include_directories(sdlgl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
