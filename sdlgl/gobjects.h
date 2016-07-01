#pragma once
#include "gobject.h"
#include "hash_map.h"
#include "render_states.h"
#include "physics/col_world.h"
#include <stdlib.h>

typedef struct {
	hash_map_t* map;
	gobject_t* data;
	render_states_t* render_states;
	col_world_t* col_world;
} gobjects_t;

gobjects_t* gobjects_new(size_t size, render_states_t* render_states);
gobject_t* gobjects_add(gobjects_t* gobjects, const char* name);
void gobjects_remove(gobjects_t* gobjects, const char* name);
gobject_t* gobjects_get(gobjects_t* gobjects, const char* name);
void gobjects_apply(gobjects_t* gobjects, float dt);
gobject_t* gobjects_add_drawable(gobjects_t* gobjects, const char* name, const char* rs_name);
gobject_t* gobjects_add_draw_phys(gobjects_t* gobjects, const char* name, const char* rs_name, int col_type);
