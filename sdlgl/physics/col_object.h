#pragma once

#include "../data_types.h"

#define PHYSICS_TYPE_STATIC 0
#define PHYSICS_TYPE_RIGID 1

typedef struct {
	mat4 transform;
	vec3 velocity;
	vec3 angular_velocity;
	float mass;
	float inertia;
	int physics_type; /* Rigid body, Static */
	void* user_ptr;
} col_object_t;

void col_object_init(col_object_t* col_object);
