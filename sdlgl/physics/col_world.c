#include "col_world.h"
#include <stdlib.h>

int COL_SHAPES_SIZE[NUM_COL_SHAPES] = {
	sizeof(col_shape_sphere_t),
	sizeof(col_shape_plane_t),
	sizeof(col_shape_box_t)
};

col_world_t* col_world_new() {
	int i;
	col_world_t* col_world = malloc(sizeof(col_world_t));
	for(i = 0; i < NUM_COL_SHAPES; ++i) {
		nf_set_init(&col_world->col_sets[i],
				NF_SET_DEFAULT_SIZE,
				COL_SHAPES_SIZE[i]);
	}
	return col_world;
}

int col_world_add(col_world_t* col_world, int col_shape) {
	return nf_set_add(&col_world->col_sets[col_shape]);
}

void col_world_apply_acceleration(col_world_t* col_world, vec3 acc, float dt) {
	int i, j;
	col_object_t* cobj;
	vec3 imp;
	vec3muls(imp, acc, dt);
	for (i = 0; i < NUM_COL_SHAPES; ++i) {
		for (j = 0; j < col_world->col_sets[i].len; ++j) {
			cobj = nf_set_at(col_world->col_sets + i, j);
			if (cobj->physics_type != PHYSICS_TYPE_STATIC) {
				vec3add(cobj->velocity, cobj->velocity, imp);
			}
		}
	}
}

void col_world_apply_velocity(col_world_t* col_world, float dt) {
	int i, j;
	col_object_t* cobj;
	for (i = 0; i < NUM_COL_SHAPES; ++i) {
		for (j = 0; j < col_world->col_sets[i].len; ++j) {
			cobj = nf_set_at(col_world->col_sets + i, j);
			if (cobj->physics_type != PHYSICS_TYPE_STATIC) {
				/* Linear */
				vec3 imp;
				vec3muls(imp, cobj->velocity, dt);
				vec3add(cobj->transform + 12,
						cobj->transform + 12,
						imp); /* Add impulse to the last column (position) */
				/* Angular */
				VmathMatrix3 rot_imp, rot, rot_after;
				vmathM4GetUpper3x3(&rot, (VmathMatrix4*)cobj->transform);
				vec3muls(imp, cobj->angular_velocity, dt);
				vmathM3MakeRotationZYX(&rot_imp, (VmathVector3*)imp);
				vmathM3Mul(&rot_after, &rot_imp, &rot);
				vmathM4SetUpper3x3((VmathMatrix4*)cobj->transform, &rot_after);
			}
		}
	}
}

void col_world_resolve_col(col_world_t* col_world) {
/* Ignore incompatible pointer type for this block.
 * Maybe functions' arguments' types should be changed
 * to make it cleaner.
 * */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
	int (*functions[])(void*, void*, collision_t*) = {
		col_detect_sphere_sphere,
		col_detect_sphere_plane,
		col_detect_sphere_box,
		col_detect_plane_plane,
		col_detect_plane_box,
		col_detect_box_box
	};
#pragma GCC diagnostic pop

	int i, j, k, l;
	int function_index = 0;

	nf_set_t* set0;
	nf_set_t* set1;

	set0 = &col_world->col_sets[COL_SPHERE];
	set1 = &col_world->col_sets[COL_PLANE];

	col_object_t* c0;
	col_object_t* c1;

	collision_t collision;

	for(i = 0; i < NUM_COL_SHAPES; ++i) {
		for(j = i; j < NUM_COL_SHAPES; ++j) {
			set0 = &col_world->col_sets[i];
			set1 = &col_world->col_sets[j];
			if (i == j) { /* Same collision shape */
				for(k = 0; k < set0->len - 1; ++k) {
					c0 = nf_set_at(set0, k);
					for(l = k + 1; l < set1->len; ++l) {
						c1 = nf_set_at(set1, l);
						if (col_detect_bound(c0, c1) && 
								functions[function_index](c0, c1, &collision) ) {
							col_res(c0, c1, &collision);
						}
					}
				}
			} else { /* Different collision shapes */
				for(k = 0; k < set0->len; ++k) {
					c0 = nf_set_at(set0, k);
					for(l = 0; l < set1->len; ++l) {
						c1 = nf_set_at(set1, l);
						if (functions[function_index](c0, c1, &collision) ) {
							col_res(c0, c1, &collision);
						}
					}
				}
			}
			function_index++; /* Switch to the next detection function */
		}
	}
}
