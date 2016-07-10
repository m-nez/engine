#pragma once

#include <math.h>
#include <stdio.h>
#include "vectormath/vectormath_aos.h"
/* Matrices are colummn major */
typedef float mat4[16];
typedef float mat3[9];
typedef float vec4[4];
typedef float vec3[3];

void mat4invert(mat4 m, mat4 inv);
void mat4projection(mat4 m, int width, int height, float near, float far);
void mat4identity(mat4 m);
void mat4mul(mat4 a, mat4 b, mat4 dest);
void mat4cpy(mat4 dest, mat4 src);
void mat4transpose(mat4 dest, mat4 a);
void mat4addi(mat4 a, mat4 b); /* Add inplace */
void mat4slerp(mat4 a, mat4 b, float x, mat4 dest);
void mat4setrot(mat4 a, mat3 rot);

void mat3mul(mat3 a, mat3 b, mat3 dest);
void mat3transpose(mat4 dest, mat3 a);
void mat3frommat4(mat3 dest, mat4 src);
void mat3axis_angle(mat3 a, vec3 axis, float* angle);
void mat3from_axis_angle(mat3 a, vec3 axis, float angle);

static inline void vec3normalize(vec3 a);
static inline float vec3dot(vec3 a, vec3 b);

static inline void vec3cross(vec3 dest, vec3 a, vec3 b);
static inline void vec3add(vec3 dest, vec3 a, vec3 b);
static inline void vec3mul(vec3 dest, vec3 a, vec3 b);
static inline void vec3muls(vec3 dest, vec3 a, float b);
static inline void vec3lerp(vec3 dest, vec3 a, vec3 b, float t);
static inline void vec3sub(vec3 dest, vec3 a, vec3 b); /* a - b*/

#include "data_types_inl.h"
