#pragma once

#include "col_shape.h"

typedef struct {
	vec3 intersection;
	vec3 normal; /* From object 0 to object 1 */
	float penetration; /* Magnitude of minimal translation vector */
} collision_t;
/* Returns 1 if they collide, 0 if they don't */
static inline int col_detect_sphere_sphere(
		col_shape_sphere_t* s0,
		col_shape_sphere_t* s1,
		collision_t* collision);

static inline int col_detect_sphere_plane(
		col_shape_sphere_t* s,
		col_shape_plane_t* p,
		collision_t* collision);

#include "col_detect_inl.h"
