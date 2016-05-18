#pragma once

#include "data_types.h"

#include <stdio.h>

/* Matrices for keyframes are stored in bone major order
 * b0(t0), b0(t1), ..., b0(tn), b1(t0), b1(t1), ... 
 * keyframes contains num_keyframes * num_bones matrices */
typedef struct {
	unsigned int num_bones;
	unsigned int num_keyframes;
	float* times;
	mat4* keyframes; /* bone major */
} anim_t;

anim_t* anim_new_from_file(const char* filename);
void anim_from_file(anim_t* anim, const char* filename);

/* Free the internal structure */
void anim_delete(anim_t* anim);

void anim_eval(anim_t* anim, float time, mat4* bones);
