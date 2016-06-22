#pragma once
#include "draw.h"
#include "render_state.h"
#include "anim.h"

typedef struct {
	render_state_t* render_state;
	char* name; /* Uniqe identifier */
	dobject_t* dobject;
	/* TODO Add physics object */
	mat4 transform;
	mat4* parent; /* mat4* so it isn't allocated as a part of the gobject_t */
	mat4* mat; /* Matrix updated with tranform matrix ex. camera */
	anim_t* bone_anim;
	float bone_anim_time;
} gobject_t;

void gobject_init(gobject_t* gobject);
void gobject_apply(gobject_t* gobject, float dt);
void gobject_move_xyz(gobject_t* gobject,
		float x, float y, float z);
void gobject_attach_mat(gobject_t* gobject, mat4 mat);
void gobject_set_anim(gobject_t* gobject, anim_t* anim, float time);
