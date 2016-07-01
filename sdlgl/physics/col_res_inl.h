static inline float col_res_impulse(col_object_t* a, col_object_t* b, collision_t* c) {
	float restitution = 1.0f; //TODO angular
	vec3 relative_vel;
	vec3sub(relative_vel, a->velocity, b->velocity);
	/* http://rasmush.se/assets/pdf/mos_2d_report.pdf page 7 n * n (WHY)*/
	float j = -(1.0f + restitution) * vec3dot(relative_vel, c->normal);
	j /= 1.0f/a->mass + 1.0f/b->mass;// +
	return j;
}

static inline void col_res_rigid(col_object_t* a, col_object_t* b, collision_t* c) {
	float j = col_res_impulse(a, b, c);
	vec3 dv, da;
	vec3 rap, rbp;
	vec3 can, cbn;

	vec3muls(dv, c->normal, j/a->mass);
	vec3add(a->velocity, a->velocity, dv);
	vec3sub(b->velocity, b->velocity, dv);

	vec3sub(rap, c->intersection, a->transform + 12);
	vec3sub(rbp, c->intersection, b->transform + 12);
	
	vec3cross(can, rap, c->normal);
	vec3cross(cbn, rbp, c->normal);

	vec3muls(da, can, j/a->inertia);
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
