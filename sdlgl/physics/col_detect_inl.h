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
	/* We don't want to resolve collision between planes
	 * So we don't detect them */
	return 0;
}

/* Project points of a convex shape onto an axis.
 * At least one point must be given.
 * Each point will be parametrized by a single scalar t.
 * t_min holds the minimal t and t_max holds the maximal.
 */
static inline void separating_axis_project(
		vec3 axis, vec3* points, int num_points,
		float* t_min, float* t_max) {
	int i;
	float d;
	*t_min = vec3dot(axis, points[0]);
	*t_max = *t_min;
	for(i = 1; i < num_points; ++i) {
		d = vec3dot(axis, points[i]);
		if (d > *t_max) {
			*t_max = d;
		} else if (d < *t_min) {
			*t_min = d;
		}
	}
}

static inline float overlap(float a_min, float a_max, float b_min, float b_max) {
	return fmin(a_max, b_max) - fmax(a_min, b_min);
}

static inline int separating_axis_detect(
		vec3* axes, int num_axes,
		vec3* a_points, int a_num_points, 
		vec3* b_points, int b_num_points, 
		float* mtv_mag, int* mtv_index) {
	/* All axes are assumed to be normalized */
	int i;
	float a_min, a_max;
	float b_min, b_max;
	float ov;
	*mtv_mag = 100000000; /* minimal overlap */

	for (i = 0; i < num_axes; ++i) {
		separating_axis_project(axes[i], a_points, a_num_points, &a_min, &a_max);
		separating_axis_project(axes[i], b_points, b_num_points, &b_min, &b_max);

		ov = overlap(a_min, a_max, b_min, b_max);
		if (ov <= 0) {
			return 0;
		}
		if (ov < *mtv_mag) {
			*mtv_mag = ov;
			*mtv_index = i;
		}
	}
	return 1;
}

/* Get points comprising the box */
static inline void box_get_points(vec3* points, col_shape_box_t* box) {
	int i;
	float a = box->dimensions[0]/2;
	float b = box->dimensions[1]/2;
	float c = box->dimensions[2]/2;

	mat3 rot;
	vec3 tmp;

	vec3set(points[0], - a, - b, - c);
	vec3set(points[1], - a, - b, + c);
	vec3set(points[2], - a, + b, - c);
	vec3set(points[3], - a, + b, + c);
	vec3set(points[4], + a, - b, - c);
	vec3set(points[5], + a, - b, + c);
	vec3set(points[6], + a, + b, - c);
	vec3set(points[7], + a, + b, + c);

	mat3frommat4(rot, box->col_object.transform);

	for(i = 0; i < 8; ++i) {
		mat3mulv(tmp, rot, points[i]);
		vec3add(points[i], tmp, box->col_object.transform + 12);
	}
}

/* Find parameters t and u such that
 * for n1(normal1), p1(point1) and n2(normal2), p2(point2)
 * p1 + n1 * t is closest possible to p2 + n2 * u.
 */
static inline void lines_closest_points(vec3 n1, vec3 p1, vec3 n2, vec3 p2, float* t, float* u) {
	vec3 p;
	float pn1, pn2;
	float n1n1, n2n2;
	float n1n2;

	vec3sub(p, p1, p2);
	pn1 = vec3dot(p, n1);
	pn2 = vec3dot(p, n2);
	n1n1 = vec3dot(n1, n1);
	n2n2 = vec3dot(n2, n2);
	n1n2 = vec3dot(n1, n2);

	*t = (pn2 * n1n2) / (n2n2 * n1n1) - (pn1)/(n1n1);
	*t /= 1 - (n1n2 * n1n2) / (n1n1 * n2n2);
	*u = (*t * n1n2 + pn2) / n2n2;
}

/* Return 1 if p is in front of plane, 0 otherwise */
static inline int plane_front(vec3 p, vec3 normal, vec3 point) {
	vec3 tmp;
	vec3sub(tmp, p, point);
	return vec3dot(tmp, normal) > 0;
}

static inline void plane_line_intersect(vec3 out, vec3 pn, vec3 pp, vec3 ln, vec3 lp) {
	int i;
	float d = -vec3dot(pn, pp);
	float t = (-d - vec3dot(pn, lp)) / vec3dot(pn, ln);
	for(i = 0; i < 3; ++i) {
		out[i] = lp[i] + ln[i] * t;
	}
}

/* Clip a polygon using a plane defined by it's normal and a point */
static inline void clip_polygon(vec3* points_out, vec3* points_in, int points_len, vec3 normal, vec3 point) {
	int i;
	vec3 tmp;
	for(i = 0; i < points_len; ++i) {
		/* point is on the backside of the plane and next point is on the front side */ 
		if (!plane_front(points_in[i], normal, point)) {
			if( plane_front(points_in[(i + 1) % points_len], normal, point) ) {
				/* Set the point outside the plane to the intersection with the edge */
				vec3sub(tmp, points_in[i], points_in[(i + 1) % points_len]); /* direction of the edge */
				plane_line_intersect(points_out[i], normal, point, tmp, points_in[i]);
			} else if ( plane_front(points_in[(i - 1) % points_len], normal, point) ) {
				vec3sub(tmp, points_in[i], points_in[(i - 1) % points_len]); /* direction of the edge */
				plane_line_intersect(points_out[i], normal, point, tmp, points_in[i]);
			}
		}
		else {
			vec3cpy(points_out[i], points_in[i]);
		}
	}
}

static inline void other_axes(int axis, int* a1, int* a2) {
	if (axis == 0) {
		*a1 = 1;
		*a2 = 2;
	} else if (axis == 1) {
		*a1 = 0;
		*a2 = 2;
	} else {
		*a1 = 0;
		*a2 = 1;
	}
}

static inline void face_points(col_shape_box_t* b, vec3 fcenter, int axis, vec3* points) {
	int a1, a2;
	vec3 offset1;
	vec3 offset2;
	other_axes(axis, &a1, &a2);
	vec3muls(offset1, b->col_object.transform + 4 * a1, b->dimensions[a1]/2);
	vec3muls(offset2, b->col_object.transform + 4 * a2, b->dimensions[a2]/2);

	vec3add(points[0], fcenter, offset1);
	vec3sub(points[1], points[0], offset2);
	vec3add(points[0], points[0], offset2);

	vec3neg(offset1, offset1);
	vec3neg(offset2, offset2);

	vec3add(points[2], fcenter, offset1);
	vec3sub(points[3], points[2], offset2);
	vec3add(points[2], points[2], offset2);
}

static inline int col_detect_box_box(
		col_shape_box_t* b0,
		col_shape_box_t* b1,
		collision_t* collision) {

	vec3 points[2][8];
	int num_points = 8;
	vec3 axes[15];
	int num_axes;
	float mtv_mag;
	int mtv_index;
	int axis_indexes[15];
	int i, j;
	float* normal; /* Pointer to vec3 */

	/* Setting points in world space */
	box_get_points(points[0], b0);
	box_get_points(points[1], b1);

	/* Box face normals */
	vec3cpy(axes[0], b0->col_object.transform + 0);
	vec3cpy(axes[1], b0->col_object.transform + 4);
	vec3cpy(axes[2], b0->col_object.transform + 8);
	vec3cpy(axes[3], b1->col_object.transform + 0);
	vec3cpy(axes[4], b1->col_object.transform + 4);
	vec3cpy(axes[5], b1->col_object.transform + 8);
	for(i = 0; i < 6; ++i) {
		axis_indexes[i] = i;
	}
	/* Cross product of edges */
	num_axes = 6;
	for(i = 0; i < 3; ++i) {
		for(j = 0; j < 3; ++j) {
			vec3cross(axes[num_axes],
					b0->col_object.transform + i * 4,
					b1->col_object.transform + j * 4);
			if (!vec3isnull(axes[num_axes], 0.00001)) {
				/* Add only cross products of edges which are not paralel.
				 * Cross product has to be normalized */
				vec3normalize(axes[num_axes]);
				axis_indexes[num_axes] = 6 + i * 3 + j;
				num_axes++;
			}
		}
	}

	/* No collision */
	if (!separating_axis_detect(
				axes, num_axes,
				points[0], num_points,
				points[1], num_points,
				&mtv_mag, &mtv_index)) {
		return 0;
	}

	normal = axes[axis_indexes[mtv_index]];
	if (mtv_index > 5) {
		/* MTV axis is one created by the cross product of eges */
		int b0_index = (axis_indexes[mtv_index] - 6) / 3;
		int b1_index = (axis_indexes[mtv_index] - 6) % 3;
		int sign;
		float t;
		float u;

		vec3 tmp;
		vec3 p0; /* point on edge of box 0 */
		vec3 p1; /* point on edge of box 1 */
		vec3cpy(p0, b0->col_object.transform + 12);
		vec3cpy(p1, b1->col_object.transform + 12);

		for(i = 0; i < 3; ++i) {
			sign = vec3dot(b0->col_object.transform + 4 * i, normal) > 0 ? 1 : -1;
			vec3muls(tmp, b0->col_object.transform + 4 * i, sign * b0->dimensions[i]/2.0);
			vec3add(p0, p0, tmp);
		}

		for(i = 0; i < 3; ++i) {
			/* Sign is flipped because normal is from b0 to b1 */
			sign = vec3dot(b1->col_object.transform + 4 * i, normal) > 0 ? -1 : 1;
			vec3muls(tmp, b1->col_object.transform + 4 * i, sign * b1->dimensions[i]/2.0);
			vec3add(p1, p1, tmp);
		}

		lines_closest_points(
				b0->col_object.transform + 4 * b0_index, p0, 
				b1->col_object.transform + 4 * b1_index, p1, 
				&t, &u);
		/* Calculate p0 as closest point on edge of box 0 */
		vec3muls(tmp, b0->col_object.transform +  4 * b0_index, t);
		vec3add(p0, p0, tmp);
		/* Calculate p1 as closest point on edge of box 1 */
		vec3muls(tmp, b1->col_object.transform +  4 * b1_index, u);
		vec3add(p1, p1, tmp);

		/* Set collision intersection as midpoint between p0 and p1 */
		vec3lerp(collision->intersection, p0, p1, 0.5);
		vec3cpy(collision->normal, normal);

		return 1;
	}
	col_shape_box_t* a;
	col_shape_box_t* b;
	int sign;
	int axis; /* Index of the reference face axis */
	int par_axis = 0; /* Index of the most parallel axis on box b */
	float max_dot = -1;
	vec3 ref_n; /* Reference face normal */
	vec3 ref_p; /* Center of reference face */
	vec3 inc_n; /* Incident face normal */
	vec3 inc_p; /* Center of incident face */
	vec3 ab; /* Vector form center of a to center of b */
	vec3 offset;
	vec3 facep[4]; /* Points on the incident face */
	if (mtv_index < 3) { /* Face on box 0 is the reference face */
		a = b0;
		b = b1;
	} else { /* Face on box 0 is the reference face */
		a = b1;
		b = b0;
	}
	axis = mtv_index % 3;
	vec3cpy(ref_n, a->col_object.transform + 4 * axis);

	/* Find the most parallel face on the box b */
	for(i = 0; i < 3; ++i) {
		float d = vec3dot(a->col_object.transform + 4 * axis,
				b->col_object.transform + 4 * i);
		if (d > max_dot) {
			par_axis = i;
			max_dot = d;
		}
	}
	vec3cpy(inc_n, b->col_object.transform + 4 * par_axis);

	/* Find the reference face center */
	vec3sub(ab, b->col_object.transform + 12, a->col_object.transform + 12);
	sign = vec3dot(ab, ref_n) > 0 ? 1 : -1;
	vec3muls(offset, ref_n, sign * a->dimensions[axis] / 2);
	vec3add(ref_p, a->col_object.transform + 12, offset);
	vec3muls(ref_n, ref_n, sign); /* Correct reference face normal */

	/* Find the incident face center */
	sign = vec3dot(ab, inc_n) > 0 ? -1 : 1;
	vec3muls(offset, inc_n, sign);
	vec3muls(offset, inc_n, sign * b->dimensions[par_axis] / 2);
	vec3add(inc_p, b->col_object.transform + 12, offset);
	vec3muls(inc_n, inc_n, sign); /* Correct incident face normal */

	/* Find incident face points */
	vec3 n;
	vec3 p;
	int a1, a2;

	face_points(b, inc_p, par_axis, facep);
	other_axes(axis, &a1, &a2);

	vec3neg(n, a->col_object.transform + 4 * a1);
	vec3muls(offset, n, -a->dimensions[a1]/2);
	vec3add(p, ref_p, offset);
	clip_polygon(facep, facep, 4, n, p);

	vec3sub(p, ref_p, offset);
	vec3neg(n, n);
	clip_polygon(facep, facep, 4, n, p);

	vec3neg(n, a->col_object.transform + 4 * a2);
	vec3muls(offset, n, -a->dimensions[a2]/2);
	vec3add(p, ref_p, offset);
	clip_polygon(facep, facep, 4, n, p);

	vec3sub(p, ref_p, offset);
	vec3neg(n, n);
	clip_polygon(facep, facep, 4, n, p);

	vec3cpy(collision->normal, normal);

	/* Set collision intersection as the average of collision points */
	vec3set(collision->intersection, 0, 0, 0);
	for(i = 0; i < 4; ++i) {
		vec3add(collision->intersection, collision->intersection, facep[i]);
	}
	vec3muls(collision->intersection, collision->intersection, 1.0/4.0);

	return 1;
}

static inline int col_detect_plane_box(
		col_shape_plane_t* p,
		col_shape_box_t* b,
		collision_t* collision) {
	//TODO
	return 0;
}

static inline int col_detect_sphere_box(
		col_shape_plane_t* p,
		col_shape_box_t* b,
		collision_t* collision) {
	//TODO
	return 0;
}
