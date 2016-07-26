#include "data_types.h"
#include <stdio.h>
#include <math.h>
#include "vectormath/vectormath_aos.h"

void mat4invert(mat4 m, mat4 invOut) {
	vmathM4Inverse( /* ABI for VmathMatrix4 is the same as mat4 */
			(VmathMatrix4*) invOut,
			(VmathMatrix4*) m);
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

void mat4mul(mat4 dest, mat4 a, mat4 b) {
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

void mat4transpose(mat4 dest, mat4 a) {
	int i;
	for(i = 0; i < 4; ++i) {
		dest[0 + i * 4] = a[0 + i];
		dest[1 + i * 4] = a[4 + i];
		dest[2 + i * 4] = a[8 + i];
		dest[3 + i * 4] = a[12+ i];
	}
}

void mat4setrot(mat4 a, mat3 rot) {
	int i;
	for (i = 0; i < 3; ++i) {
		a[0 + i*4] = rot[0 + i*3];
		a[1 + i*4] = rot[1 + i*3];
		a[2 + i*4] = rot[2 + i*3];
	}
}

void mat3mul(mat3 dest, mat3 a, mat3 b) {
	int i, r, c;
	for (i = 0; i < 9; ++i) {
		dest[i] = 0;
	}
	
	for (r = 0; r < 3; ++r) {
		for (c = 0; c < 3; ++c) {
			for (i = 0; i < 3; ++i)
				dest[c * 3 + r] += a[3 * i + r] * b[3 * c + i];
		}
	}
}

void mat3transpose(mat3 dest, mat3 a) {
	int i;
	for(i = 0; i < 3; ++i) {
		dest[0 + i * 3] = a[0 + i];
		dest[1 + i * 3] = a[3 + i];
		dest[2 + i * 3] = a[6 + i];
	}
}

void mat3frommat4(mat3 dest, mat4 src) {
	int i;
	for (i = 0; i < 3; ++i) {
		dest[0 + i*3] = src[0 + i*4];
		dest[1 + i*3] = src[1 + i*4];
		dest[2 + i*3] = src[2 + i*4];
	}
}

void mat3axis_angle(mat3 a, vec3 axis, float* angle) {
	float den = sqrt(
			POW2(a[5] - a[7]) +
			POW2(a[6] - a[2]) +
			POW2(a[1] - a[3]));
	*angle = acos((a[0] + a[3] + a[6]) / 2.0);
	axis[0] = (a[5] - a[7])/den;
	axis[1] = (a[6] - a[2])/den;
	axis[2] = (a[1] - a[3])/den;
}

void mat3from_axis_angle(mat3 a, vec3 axis, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	float t = 1.0 - c;
	float x = axis[0];
	float y = axis[1];
	float z = axis[2];
	a[0] = t*x*x + c;
	a[1] = t*x*y + z*s;
	a[2] = t*x*z - y*s;

	a[3] = t*x*y - z*s;
	a[4] = t*y*y + c;
	a[5] = t*y*z + x*s;

	a[6] = t*x*z + y*s;
	a[7] = t*y*z - x*s;
	a[8] = t*z*z + c;
}

void mat4slerp(mat4 a, mat4 b, float x, mat4 dest) {
	int i;
	VmathMatrix3 m;
	VmathQuat q1, q2, q3;

	vmathM3MakeFromCols(&m,
			(VmathVector3*)(a + 0),
			(VmathVector3*)(a + 4),
			(VmathVector3*)(a + 8));
	vmathQMakeFromM3(&q1, &m);

	vmathM3MakeFromCols(&m,
			(VmathVector3*)(b + 0),
			(VmathVector3*)(b + 4),
			(VmathVector3*)(b + 8));
	vmathQMakeFromM3(&q2, &m);

	vmathQSlerp(&q3, x, &q1, &q2);
	vmathM3MakeFromQ(&m, &q3);

	dest[0 + 0 * 4] = m.col0.x;
	dest[1 + 0 * 4] = m.col0.y;
	dest[2 + 0 * 4] = m.col0.z;

	dest[0 + 1 * 4] = m.col1.x;
	dest[1 + 1 * 4] = m.col1.y;
	dest[2 + 1 * 4] = m.col1.z;

	dest[0 + 2 * 4] = m.col2.x;
	dest[1 + 2 * 4] = m.col2.y;
	dest[2 + 2 * 4] = m.col2.z;

	dest[3] = 0;
	dest[7] = 0;
	dest[11] = 0;
	dest[15] = 1;
	for (i = 12; i < 15; ++i) {
		dest[i] = a[i] * (1 - x) + b[i] * x;
	}
}
