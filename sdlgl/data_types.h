#pragma once

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
void mat4addi(mat4 a, mat4 b);
void mat4slerp(mat4 a, mat4 b, float x, mat4 dest);

float vec3dot(vec3 a, vec3 b);
void vec3cross(vec3 a, vec3 b, vec3 c);
