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
