#include "gobject.h"
#include <stdlib.h>

void gobject_init(gobject_t* gobject) {
	gobject->render_state = NULL;
	gobject->dobject = NULL;
	gobject->parent = NULL;
	gobject->name = NULL;
	mat4identity(gobject->transform);
}

void gobject_apply(gobject_t* gobject) {
	memcpy(	gobject->dobject->model_mat,
			gobject->transform,
			sizeof(gobject->transform));
}
