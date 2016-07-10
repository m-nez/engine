#pragma once

#include "col_object.h"

#define COL_SPHERE 0
#define COL_PLANE 1
#define COL_BOX 2
#define NUM_COL_SHAPES 3

typedef struct {
	col_object_t col_object; /* Make sure col_object is at the begining of every col_shape */
	/* From this line - shape specific things */
	float radius;
} col_shape_sphere_t;

typedef struct {
	col_object_t col_object;
	/* Plane:
	 * Normal = col_object.trasform + 8
	 * Point = col_object.transform + 12
	 */
} col_shape_plane_t;

typedef struct {
	col_object_t col_object;
	vec3 dimensions;
} col_shape_box_t;

void col_shape_sphere_init(col_shape_sphere_t* col_shape);

void col_shape_sphere_calc_inertia(col_shape_sphere_t* col_shape);

void col_shape_box_init(col_shape_box_t* col_shape);

void col_shape_box_calc_inertia(col_shape_box_t* col_shape);

void col_shape_init(void* col_shape, int col_type);

