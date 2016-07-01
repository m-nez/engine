#pragma once

#include "../nf_set.h"
#include "col_shape.h"
#include "col_res.h"

#define NF_SET_DEFAULT_SIZE 256

typedef struct {
	nf_set_t col_sets[NUM_COL_SHAPES];
} col_world_t;

col_world_t* col_world_new();

/* Returns -1 if no realloc occured */
int col_world_add(col_world_t* col_world, int col_shape);

void col_world_apply_acceleration(col_world_t* col_world, vec3 acc, float dt);

void col_world_apply_velocity(col_world_t* col_world, float dt);

void col_world_resolve_col(col_world_t* col_world);
