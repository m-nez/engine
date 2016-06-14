#pragma once
#include "draw.h"
#include "render_state.h"

typedef struct {
	render_state_t* render_state;
	char* name; /* Uniqe identifier */
	dobject_t* dobject;
	/* TODO Add physics object */
	mat4* parent;
	mat4 transform;
} gobject_t;

void gobject_init(gobject_t* gobject);
void gobject_apply(gobject_t* gobject);
