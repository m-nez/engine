#include "gobject.h"
#include <stdlib.h>

void gobject_init(gobject_t* gobject) {
	gobject->render_state = NULL;
	gobject->dobject = NULL;
	gobject->col_object = NULL;
	gobject->parent = NULL;
	gobject->name = NULL;
	gobject->mat = NULL;
	gobject->bone_anim = NULL;
	gobject->light = NULL;
	gobject->light_index = 0;
	
	mat4identity(gobject->transform);
	vec3set(gobject->scale, 1, 1, 1);
}

void gobject_apply(gobject_t* gobject, float dt) {
	int i;
	if (gobject->col_object != NULL) {
		memcpy( gobject->transform,
				gobject->col_object->transform,
				sizeof(gobject->transform));
	}
	/* Apply scale */
	for(i = 0; i < 3; ++i) {
		vec3muls(gobject->transform + 4 * i,
				gobject->transform + 4 * i,
				gobject->scale[i]);
	}
	if (gobject->dobject != NULL) {
		memcpy(	gobject->dobject->model_mat,
				gobject->transform,
				sizeof(gobject->transform));
		if (gobject->bone_anim != NULL) {
			anim_eval(gobject->bone_anim,
					gobject->bone_anim_time,
					gobject->dobject->bones);
			gobject->bone_anim_time += dt;
		}
	}
	if (gobject->mat != NULL) {
		memcpy(	gobject->mat,
				gobject->transform,
				sizeof(gobject->transform));
	}
}

void gobject_move_xyz(gobject_t* gobject,
		float x, float y, float z) {
	vec3 m;
	vec3set(m, x, y, z);
	if (gobject->col_object != NULL) {
		vec3add(gobject->col_object->transform + 12,
				gobject->col_object->transform + 12,
				m);
	} else {
		vec3add(gobject->transform + 12,
				gobject->transform + 12,
				m);
	
	}
	return;
}

void gobject_attach_mat(gobject_t* gobject, mat4 mat) {
	gobject->mat = (mat4*) mat; /* mat4 is already a pointer, so this only removes warnings */
}

void gobject_set_anim(gobject_t* gobject, anim_t* anim, float time) {
	gobject->bone_anim = anim;
	gobject->bone_anim_time = time;
}
