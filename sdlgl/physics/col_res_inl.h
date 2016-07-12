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

static inline void relative_velocity(vec3 relative_vel, col_object_t* a, col_object_t* b, collision_t* c) {
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
}

static inline float col_res_impulse(vec3 relative_vel, col_object_t* a, col_object_t* b, collision_t* c) {
	float restitution = fmin(a->restitution, b->restitution);
	vec3 point;
	vec3 a1;
	vec3 a2;

	vec3sub(point, c->intersection, a->transform + 12); 
	vec3cross(a1, point, c->normal);
	vec3cross(a1, a1, point);
	vec3muls(a1, a1, 1.0/a->inertia);

	vec3sub(point, c->intersection, b->transform + 12); 
	vec3cross(a2, point, c->normal);
	vec3cross(a2, a2, point);
	vec3muls(a2, a2, 1.0/b->inertia);

	vec3add(a1, a1, a2);

	float j = -(1.0f + restitution) * vec3dot(relative_vel, c->normal);
	j /= 1.0f/a->mass + 1.0f/b->mass + fabs(vec3dot(a1, c->normal)); /* + Some angular stuff? */
	return j;
}

static inline float col_res_friction_impulse(vec3 jnorm, vec3 rel_vel, col_object_t* a, col_object_t* b, collision_t* c) {
	float friction = (a->friction + b->friction) / 2.0;
	float dnrv = vec3dot(c->normal, rel_vel);
	float tmag;
	vec3 tangent;
	vec3muls(tangent, c->normal, dnrv);
	vec3sub(tangent, rel_vel, tangent);
	tmag = vec3mag(tangent);
	vec3norm(jnorm, tangent);
	vec3neg(jnorm, jnorm);

	return fmin(fabs(dnrv * friction), tmag);
}

static inline void col_res_apply_impulse(col_object_t* a, col_object_t* b, collision_t* c, float j, vec3 jnorm) {
	vec3 dv, da;
	vec3 rap, rbp;
	vec3 can, cbn;

	/* Linear */
	vec3muls(dv, jnorm, j/a->mass);
	vec3add(a->velocity, a->velocity, dv);
	vec3muls(dv, jnorm, j/b->mass);
	vec3sub(b->velocity, b->velocity, dv);

	/* Angular */
	vec3sub(rap, c->intersection, a->transform + 12);
	vec3sub(rbp, c->intersection, b->transform + 12);
	
	vec3cross(can,  jnorm, rap);
	vec3cross(cbn,  jnorm, rbp);

	vec3muls(da, can, -j/a->inertia);
	vec3add(a->angular_velocity, a->angular_velocity, da);

	vec3muls(da, cbn, j/b->inertia);
	vec3add(b->angular_velocity, b->angular_velocity, da);
}

static inline void col_res_rigid(col_object_t* a, col_object_t* b, collision_t* c) {
	vec3 jnorm;
	vec3 rel_vel;
	float j;

	relative_velocity(rel_vel, a, b, c);
	j = col_res_impulse(rel_vel, a, b, c);
	col_res_apply_impulse(a, b, c, j, c->normal);
	j = col_res_friction_impulse(jnorm, rel_vel, a, b, c);
	col_res_apply_impulse(a, b, c, j, jnorm);

	/* Apply minimal translation vector */
	vec3 mtv;
	vec3 mtva;
	float m = a->mass + b->mass;
	vec3muls(mtv, c->normal, -c->penetration);
	vec3muls(mtva, mtv, b->mass/m);
	vec3add(a->transform + 12, a->transform + 12, mtva);
	vec3muls(mtva, mtv, - a->mass/m);
	vec3add(b->transform + 12, b->transform + 12, mtva);
}

static inline void col_res_static(col_object_t* a, col_object_t* b, collision_t* c) {
	float m = b->mass;
	float i = b->inertia;
	const float inf = 10000000000;
	b->mass = inf;
	b->inertia = inf;
	vec3set(b->velocity, 0, 0, 0);
	vec3set(b->angular_velocity, 0, 0, 0);
	col_res_rigid(a, b, c);
	b->mass = m;
	b->inertia = i;
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
