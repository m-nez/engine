#pragma once

static inline int col_detect_sphere_sphere(
		col_shape_sphere_t* s0,
		col_shape_sphere_t* s1,
		collision_t* collision) {
	vec3lerp(collision->intersection,
			s0->col_object.transform + 12,
			s1->col_object.transform + 12,
			s0->radius/(s0->radius+s1->radius));
	vec3sub(collision->normal,
			s1->col_object.transform + 12,
			s0->col_object.transform + 12);
	vec3normalize(collision->normal);
	return POW2(s0->radius + s1->radius) >=

		POW2(	s0->col_object.transform[12] -
				s1->col_object.transform[12]) +
		POW2(	s0->col_object.transform[13] -
				s1->col_object.transform[13]) +
		POW2(	s0->col_object.transform[14] -
				s1->col_object.transform[14]);
}

static inline int col_detect_sphere_plane(
		col_shape_sphere_t* s,
		col_shape_plane_t* p,
		collision_t* collision) {
	float A = p->col_object.transform[8]; /* Z Axis is plane's normal */
	float B = p->col_object.transform[9];
	float C = p->col_object.transform[10];

	float x = s->col_object.transform[12];
	float y = s->col_object.transform[13];
	float z = s->col_object.transform[14];

	/* transform location belongs to the plane */
	float D = -A*p->col_object.transform[12] -
				B*p->col_object.transform[13] -
				C*p->col_object.transform[14];
	float d2 = POW2(A*x+B*y+C*z+D);
	d2 /= A*A + B*B + C*C;

	vec3 nnorm;
	vec3neg(nnorm, p->col_object.transform + 8);
	vec3norm(collision->normal, nnorm);
	vec3muls(nnorm, nnorm, s->radius);
	vec3add(collision->intersection, s->col_object.transform + 12, nnorm);
	return d2 <= s->radius;
}

static inline int col_detect_plane_plane(
		col_shape_plane_t* p0,
		col_shape_plane_t* p1,
		collision_t* collision) {
	/* We don't want to resolve collision between planes */
	return 0;
}
