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