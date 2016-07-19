static inline float vec3dot(vec3 a, vec3 b) {
	float s = 0;
	int i;
	for(i = 0; i < 3; ++i) {
		s += a[i] * b[i];
	}
	return s;
}

static inline void vec3cross(vec3 dest, vec3 a, vec3 b) {
	dest[0] = a[1]*b[2] - a[2] * b[1];
	dest[1] = a[2]*b[0] - a[0] * b[2];
	dest[2] = a[0]*b[1] - a[1] * b[0];
}

static inline void vec3print(vec3 a) {
	printf("%g %g %g\n", a[0], a[1], a[2]);
}

static inline void vec3add(vec3 dest, vec3 a, vec3 b) {
	dest[0] = a[0] + b[0];
	dest[1] = a[1] + b[1];
	dest[2] = a[2] + b[2];
}

static inline void vec3mul(vec3 dest, vec3 a, vec3 b) {
	dest[0] = a[0] * b[0];
	dest[1] = a[1] * b[1];
	dest[2] = a[2] * b[2];
}

static inline void vec3muls(vec3 dest, vec3 a, float b) {
	dest[0] = a[0] * b;
	dest[1] = a[1] * b;
	dest[2] = a[2] * b;
}

static inline float POW2(float x) {
	return x * x;
}

static inline void vec3set(vec3 v, float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

static inline void vec3lerp(vec3 dest, vec3 a, vec3 b, float t) {
	dest[0] = a[0] * (1 - t) + b[0] * t;
	dest[1] = a[1] * (1 - t) + b[1] * t;
	dest[2] = a[2] * (1 - t) + b[2] * t;
}

static inline void vec3sub(vec3 dest, vec3 a, vec3 b) {
	dest[0] = a[0] - b[0];
	dest[1] = a[1] - b[1];
	dest[2] = a[2] - b[2];
}

static inline void vec3neg(vec3 dest, vec3 a) {
	dest[0] = -a[0];
	dest[1] = -a[1];
	dest[2] = -a[2];
}

static inline void vec3norm(vec3 dest, vec3 a) {
	float mag = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	dest[0] = a[0] / mag;
	dest[1] = a[1] / mag;
	dest[2] = a[2] / mag;
}

static inline void vec3reflect(vec3 dest, vec3 a, vec3 normal) {
	float d = vec3dot(a, normal) * -2.0f;
	vec3 dn;
	vec3muls(dn, normal, d);
	vec3add(dest, a, dn);
}

static inline void vec3cpy(vec3 dest, vec3 src) {
	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = src[2];
}

static inline float vec3mag(vec3 a) {
	return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}

static inline void vec3normalize(vec3 a) {
	float m = vec3mag(a);
	a[0] /= m;
	a[1] /= m;
	a[2] /= m;
}

static inline float vec3dist(vec3 a, vec3 b) {
	return sqrt(POW2(a[0] - b[0]) + POW2(a[1] - b[1]) + POW2(a[2] - b[2]));
}

/* Check if v is null with precision margin of error eps */
static inline int vec3isnull(vec3 v, float eps) {
	if (fabs(v[0]) > eps) return 0;
	if (fabs(v[1]) > eps) return 0;
	if (fabs(v[2]) > eps) return 0;
	return 1;
}

static inline void mat3mulv(vec3 dest, mat3 m, vec3 v) {
	dest[0] = m[0] * v[0] + m[3] * v[1] + m[6] * v[2];
	dest[1] = m[1] * v[0] + m[4] * v[1] + m[7] * v[2];
	dest[2] = m[2] * v[0] + m[5] * v[1] + m[8] * v[2];
}
