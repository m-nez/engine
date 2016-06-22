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
void mat4transpose(mat4 a, mat4 dest);

/* Add inplace */
void mat4addi(mat4 a, mat4 b);
void mat4slerp(mat4 a, mat4 b, float x, mat4 dest);
void mat4setrot(mat4 a, mat3 rot);

void mat3mul(mat3 a, mat3 b, mat3 dest);
void mat3transpose(mat3 a, mat3 dest);
void mat3frommat4(mat3 dest, mat4 src);
void mat3axis_angle(mat3 a, vec3 axis, float* angle);
void mat3from_axis_angle(mat3 a, vec3 axis, float angle);

void vec3normalize(vec3 a);
float vec3dot(vec3 a, vec3 b);
void vec3cross(vec3 a, vec3 b, vec3 dest);
