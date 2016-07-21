#include "light.h"
#include <string.h>

int light_remove_at(light_t* light, int i) {
	int last = light->len - 1;
	if (i == last) {
		light->len--;
		return -1;
	}
	memcpy(light->pos + i, light->pos + last, sizeof(*light->pos));
	memcpy(light->col + i, light->col + last, sizeof(*light->col));
	memcpy(light->user_ptr + i, light->user_ptr + last, sizeof(*light->user_ptr));
	return 0;
}

int light_add(light_t* light) {
	light->len++;
	return light->len - 1;
}

void light_init_at(light_t* light, int i) {
	mat4identity(light->pos[i]);
	vec4set(light->col[i], 1, 1, 1, 1);
	light->user_ptr[i] = NULL;
}

void light_init(light_t* light) {
	int i;
	for (i = 0; i < MAX_LIGHTS; ++i) {
		light_init_at(light, i);
	}
	light->len = 0;
}
