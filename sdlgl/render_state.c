#include "render_state.h"
#include "string.h"

void dobject_init(dobject_t* dobject, render_state_t* rs) {
	int i;
	mat4identity(dobject->model_mat);
	dobject->bones = malloc( rs->num_bones * sizeof(*dobject->bones));
	for(i = 0; i < rs->num_bones; ++i) {
		mat4identity(dobject->bones[i]);
	}
	dobject->gobject_ptr = NULL;
}

dobject_t* render_state_add(render_state_t* rs) {
	if (rs->len == rs->dobjects_size) {
		rs->dobjects = realloc(rs->dobjects, sizeof(dobject_t) * 2 * rs->dobjects_size);
		rs->dobjects_size *= 2;
	}
	dobject_init(&rs->dobjects[rs->len], rs);
	rs->len++;
	return rs->dobjects + rs->len - 1;
}

/* Return -1 if the last element has been removed and no swapping ocured,
 * else return 0.
 */
int render_state_remove_at(render_state_t* rs, int index) {
	int last = rs->len - 1;
	if (index == last) { /* Last */
		rs->len--;
		return -1;
	}
	memcpy(rs->dobjects + index, rs->dobjects + last, sizeof(dobject_t) );
	rs->len--;
	return 0;
}
