#pragma once

#include "data_types.h"

typedef enum {
	light_type_point = 0,
	light_type_num
} light_type;

#define MAX_LIGHTS 32

typedef struct {
	mat4 pos[MAX_LIGHTS];
	vec4 col[MAX_LIGHTS];
	void* user_ptr[MAX_LIGHTS];
	int len;
} light_t;

int light_remove_at(light_t* light, int i);
