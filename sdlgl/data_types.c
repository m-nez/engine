#include "data_types.h"
#include <stdio.h>
#include <math.h>

/* COPIED AND MODIFED FROM MESA GLU */
void mat4invert(mat4 m, mat4 invOut) {
    float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
}

void mat4projection(mat4 m, int width, int height, float near, float far) {
	float r = (width < height ? 1.0 : (float) width/height);
	float t = (height < width ? 1.0 : (float) height/width);
	float n = near;
	float f = far;

	m[0] = n/r;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = n/t;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = (f+n)/(n-f);
	m[11] = -1;

	m[12] = 0;
	m[13] = 0;
	m[14] = 2*f*n/(n-f);
	m[15] = 0;
}

void mat4identity(mat4 m) {
	int i;
	for (i = 0; i < 16; ++i)
		m[i] = 0;
	m[0] = 1;
	m[5] = 1;
	m[10] = 1;
	m[15] = 1;
}

void mat4mul(mat4 a, mat4 b, mat4 dest) {
	int i, r, c;
	for (i = 0; i < 16; ++i) {
		dest[i] = 0;
	}
	
	for (r = 0; r < 4; ++r) {
		for (c = 0; c < 4; ++c) {
			for (i = 0; i < 4; ++i)
				dest[c * 4 + r] += a[4 * i + r] * b[4 * c + i];
		}
	}
}

void mat4cpy(mat4 dest, mat4 src) {
	int i;
	for(i = 0; i < 16; ++i) {
		dest[i] = src[i];
	}
}

void mat4addi(mat4 a, mat4 b) {
	int i;
	for(i = 0; i < 16; ++i) {
		a[i] += b[i];
	}
}

static inline void vec3spherical(vec3 v, vec3 s) {
	/* s[0] = r, s[1] = fi, s[2] = theta */
	int i;
	s[0] = 0;
	for (i = 0; i < 3; ++i) {
		s[0] += v[i] * v[i];
	}
	s[0] = sqrt(s[0]);
	s[2] = acos(v[2] / s[0]);
	s[1] = atan2(v[1], v[0]);
}

static inline void vec3fromspherical(vec3 v, vec3 s) {
	float st = sin(s[2]);
	v[0] = s[0] * st * cos(s[1]);
	v[1] = s[0] * st * sin(s[1]);
	v[2] = s[0] * cos(s[2]);
}

void mat4slerp(mat4 a, mat4 b, float x, mat4 dest) {
	/* TODO */
	int i, j;
	vec3 s[2];
	for (i = 0; i < 3; ++i) {
		vec3spherical(a + i * 4, s[0]);
		vec3spherical(b + i * 4, s[1]);
		for(j = 0; j < 3; ++j) {
			s[0][j] = s[0][j] * (1 - x) + s[1][j] * (x);
		}
		vec3fromspherical(dest + i * 4, s[0]);
	}
	for (i = 12; i < 15; ++i) {
		dest[i] = a[i] * (1 - x) + b[i] * x;
	}
}
