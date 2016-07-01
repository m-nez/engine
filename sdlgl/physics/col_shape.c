#include "col_shape.h"

void col_shape_sphere_init(col_shape_sphere_t* col_shape) {
	col_shape->radius = 1.0;
}

void col_shape_init(void* col_shape, int col_type) {
	col_object_init((col_object_t*)col_shape);
	switch (col_type) {
		case COL_SPHERE:
			col_shape_sphere_init(col_shape);
			break;
		default:/*COL_PLANE doesn't need initialization */
			break;
	}
}
