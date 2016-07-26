#include "col_object.h"

void col_object_init(col_object_t* col_object) {
	mat4identity(col_object->transform);
	vec3set(col_object->velocity, 0,0,0);
	vec3set(col_object->angular_velocity, 0,0,0);
	col_object->physics_type = PHYSICS_TYPE_STATIC;
	col_object->mass = 1.0f;
	col_object->restitution = 0.7f;
	col_object->friction = 0.8f;
	col_object->bounding_radius = 10000000000000;
}
