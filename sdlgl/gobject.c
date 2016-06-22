#include "gobject.h"
#include <stdlib.h>

void gobject_init(gobject_t* gobject) {
	gobject->render_state = NULL;
	gobject->dobject = NULL;
	gobject->parent = NULL;
	gobject->name = NULL;
	gobject->mat = NULL;
	gobject->bone_anim = NULL;
	mat4identity(gobject->transform);
}

void gobject_apply(gobject_t* gobject, float dt) {
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
	gobject->transform[12] += x;
	gobject->transform[13] += y;
	gobject->transform[14] += z;
}

void gobject_attach_mat(gobject_t* gobject, mat4 mat) {
	gobject->mat = (mat4*) mat; /* mat4 is already a pointer, so this only removes warnings */
}

void gobject_set_anim(gobject_t* gobject, anim_t* anim, float time) {
	gobject->bone_anim = anim;
	gobject->bone_anim_time = time;
}
