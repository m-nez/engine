static inline void point_linear_velocity_from_angular(vec3 dest, vec3 p, vec3 a) {
	vec3 x = {a[0], 0, 0};
	vec3 y = {0, a[1], 0};
	vec3 z = {0, 0, a[2]};

	vec3 tmp;

	vec3set(dest, 0, 0 , 0);
	vec3cross(tmp, x, p);
	vec3add(dest, dest, tmp);
	vec3cross(tmp, y, p);
	vec3add(dest, dest, tmp);
	vec3cross(tmp, z, p);
	vec3add(dest, dest, tmp);
}

static inline float col_res_impulse(col_object_t* a, col_object_t* b, collision_t* c) {
	float restitution = fmin(a->restitution, b->restitution);
	vec3 relative_vel;
	vec3 ang_lin_vel;
	vec3 point;

	/* Relative velocity of centers of a and b */
	vec3sub(relative_vel, a->velocity, b->velocity);

	/* Location of the intersection point relative to the object a */
	vec3sub(point, c->intersection, a->transform + 12); 
	point_linear_velocity_from_angular(ang_lin_vel, point, a->angular_velocity);

	vec3add(relative_vel, relative_vel, ang_lin_vel);

	/* Location of the intersection point relative to the object b */
	vec3sub(point, c->intersection, b->transform + 12); 
	point_linear_velocity_from_angular(ang_lin_vel, point, b->angular_velocity);

	vec3sub(relative_vel, relative_vel, ang_lin_vel);

	float j = -(1.0f + restitution) * vec3dot(relative_vel, c->normal);
	j /= 1.0f/a->mass + 1.0f/b->mass; /* + Some angular stuff? */
	return j;
}

static inline void col_res_rigid(col_object_t* a, col_object_t* b, collision_t* c) {
	float j = col_res_impulse(a, b, c);
	vec3 dv, da;
	vec3 rap, rbp;
	vec3 can, cbn;

	/* Linear */
	vec3muls(dv, c->normal, j/a->mass);
	vec3add(a->velocity, a->velocity, dv);
	vec3muls(dv, c->normal, j/b->mass);
	vec3sub(b->velocity, b->velocity, dv);

	/* Angular */
	vec3sub(rap, c->intersection, a->transform + 12);
	vec3sub(rbp, c->intersection, b->transform + 12);
	
	vec3cross(can,  c->normal, rap);
	vec3cross(cbn,  c->normal, rbp);

	//vec3print(can);

	vec3muls(da, can, -j/a->inertia);
	vec3add(a->angular_velocity, a->angular_velocity, da);

	vec3muls(da, cbn, j/b->inertia);
	vec3add(b->angular_velocity, b->angular_velocity, da);
}

static inline void col_res_static(col_object_t* a, col_object_t* b, collision_t* c) {
	vec3reflect(a->velocity, a->velocity, c->normal);
}

static inline void col_res(col_object_t* a, col_object_t* b, collision_t* c) {
	if (b->physics_type == PHYSICS_TYPE_STATIC &&
			a->physics_type == PHYSICS_TYPE_STATIC) {
		return;
	} else if (b->physics_type == PHYSICS_TYPE_STATIC) {
		col_res_static(a,b,c);
	} else if (a->physics_type == PHYSICS_TYPE_STATIC){
		vec3neg(c->normal, c->normal); /* ??? */
		col_res_static(b,a,c);
	} else {
		col_res_rigid(a,b,c);
	}
}
