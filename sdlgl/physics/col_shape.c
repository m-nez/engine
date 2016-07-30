#include "col_shape.h"

static inline void col_object_set_inertia_tensor(col_object_t* col_object, float x, float y, float z) {
	mat3diag(col_object->inertia_tensor, x, y, z);
	mat3invdiag(col_object->inertia_tensor_inv, col_object->inertia_tensor);
}

void col_shape_sphere_init(col_shape_sphere_t* col_shape) {
	col_shape->radius = 1.0;
	col_shape_sphere_calc_inertia(col_shape);
}

void col_shape_sphere_calc_inertia(col_shape_sphere_t* col_shape) {
	float tmp = 0.4 * col_shape->col_object.mass * POW2(col_shape->radius);
	col_object_set_inertia_tensor(&col_shape->col_object, tmp, tmp, tmp);
}

void col_shape_sphere_calc(col_shape_sphere_t* col_shape) {
	col_shape_sphere_calc_inertia(col_shape);
	col_shape->col_object.bounding_radius = col_shape->radius;
}

float col_shape_box_calc_bounding_radius(col_shape_box_t* col_shape) {
	return vec3mag(col_shape->dimensions) / 2.0f;
}

void col_shape_box_calc(col_shape_box_t* col_shape) {
	col_shape_box_calc_inertia(col_shape);
	col_shape->col_object.bounding_radius = col_shape_box_calc_bounding_radius(col_shape);
}

void col_shape_box_init(col_shape_box_t* col_shape) {
	vec3set(col_shape->dimensions, 1.0, 1.0, 1.0);
	col_shape_box_calc_inertia(col_shape);
}

void col_shape_box_calc_inertia(col_shape_box_t* col_shape) {
	float x = col_shape->dimensions[0];
	float y = col_shape->dimensions[1];
	float z = col_shape->dimensions[2];
	float m = col_shape->col_object.mass;
	col_object_set_inertia_tensor(&col_shape->col_object,
			m * (y*y + z*z) / 12,
			m * (x*x + z*z) / 12,
			m * (y*y + x*x) / 12);
}

void col_shape_plane_calc_inertia(col_shape_plane_t* col_shape) {
	float inf = 10000000000000;
	col_object_set_inertia_tensor(&col_shape->col_object, inf, inf, inf);
}

void col_shape_plane_init(col_shape_plane_t* col_shape) {
	col_shape->col_object.mass = 10000000000000;
	col_shape_plane_calc_inertia(col_shape);
}

void col_shape_init(void* col_shape, int col_type) {
	col_object_init((col_object_t*)col_shape);
	switch (col_type) {
		case COL_SPHERE:
			col_shape_sphere_init(col_shape);
			break;
		case COL_BOX:
			col_shape_box_init(col_shape);
			break;
		case COL_PLANE:
			col_shape_plane_init(col_shape);
		default:
			break;
	}
}
