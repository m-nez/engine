#include "col_shape.h"

void col_shape_sphere_init(col_shape_sphere_t* col_shape) {
	col_shape->radius = 1.0;
	col_shape_sphere_calc_inertia(col_shape);
}

void col_shape_sphere_calc_inertia(col_shape_sphere_t* col_shape) {
	col_shape->col_object.inertia = 2 * POW2(col_shape->radius) * col_shape->col_object.mass * M_PI / 5;
}

void col_shape_box_init(col_shape_box_t* col_shape) {
	vec3set(col_shape->dimensions, 1.0, 1.0, 1.0);
	col_shape_box_calc_inertia(col_shape);
}

void col_shape_box_calc_inertia(col_shape_box_t* col_shape) {
	float x = col_shape->dimensions[0];
	float y = col_shape->dimensions[1];
	float z = col_shape->dimensions[2];
	col_shape->col_object.inertia = col_shape->col_object.mass * (x*x + y*y + z*z) / 12;
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
		default:
			/*COL_PLANE doesn't need initialization */
			break;
	}
}
