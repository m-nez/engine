#include "gobjects.h"
#include <stdio.h>

gobjects_t* gobjects_new(size_t size, render_states_t* render_states) {
	gobjects_t* g = malloc(sizeof(gobject_t));
	g->map = hash_map_new(size);
	g->data = malloc(sizeof(gobject_t) * size);
	g->render_states = render_states;
	return g;
}

gobject_t* gobjects_add(gobjects_t* gobjects, const char* name) {
	gobject_t* gp = gobjects->data + gobjects->map->len;
	if (hash_map_get(gobjects->map, name) != NULL) {
		fprintf(stderr, "Error: Could not add gobject: gobject with name \"%s\" already exists.\n", name);
		return NULL;
	}
	gobject_init(gp);
	hash_map_set(gobjects->map, name, gp);
	return gp;
}

void gobjects_remove(gobjects_t* gobjects, const char* name) {
	gobject_t* gp = hash_map_get(gobjects->map, name);
	if (gp == NULL) {
		fprintf(stderr, "Error: Could not remove gobject: No gobject with name \"%s\" found.\n", name);
		return;
	}
	if (gp->render_state != NULL && gp->dobject != NULL) {
		dobject_t* d = gp->dobject;
		int dobject_index = d - gp->render_state->dobjects;
		if (render_state_remove_at(gp->render_state, dobject_index) != -1) {
			/* Swap has occured, so dobject pointer needs fixing */
			if (d->gobject_ptr != NULL) {
				/* swapped dobject is referenced by a gobject */
				gobject_t* g_to_update = (gobject_t*)d->gobject_ptr;
				g_to_update->dobject = d;
			}
		}

	}
	if (gp - gobjects->data != gobjects->map->len - 1) {
		/* Not the last element, so we have to swap */
		memcpy(gp, gobjects->data + gobjects->map->len - 1, sizeof(gobject_t));
		/* Update dobject's pointer */
		gp->dobject->gobject_ptr = gp;
		/* Update pointer in the hash map */
		hash_map_set(gobjects->map, gp->name, gp);
	}
	hash_map_remove(gobjects->map, name);
}

gobject_t* gobjects_get(gobjects_t* gobjects, const char* name) {
	gobject_t* gp = hash_map_get(gobjects->map, name);
	if (gp == NULL) {
		fprintf(stderr, "Warning: No gobject with name \"%s\" found.\n", name);
	}
	return gp;
}

void gobjects_apply(gobjects_t* gobjects, float dt) {
	int i;
	for( i = 0; i < gobjects->map->len; ++i) {
		gobject_apply(gobjects->data + i, dt);
	}
}

gobject_t* gobjects_add_drawable(gobjects_t* gobjects, const char* name, const char* rs_name) {
	gobject_t* g = gobjects_add(gobjects, name);
	if (g == NULL) {
		return NULL;
	}
	render_state_t* rs = hash_map_get(gobjects->render_states->map, rs_name);
	if (rs == NULL) {
		fprintf(stderr, "Warning: No render state with name \"%s\" found.\n", rs_name);
		return g;
	}
	dobject_t* dobjects_pre = rs->dobjects;
	g->dobject = render_state_add(rs);
	g->dobject->gobject_ptr = g;
	if (rs->dobjects != dobjects_pre) {
		/* Adding caused a realloc, gobjects' dobject pointers have to be updated */
		int i;
		for (i = 0; i < rs->len; ++i) {
			if (rs->dobjects[i].gobject_ptr != NULL) {
				gobject_t* gob = rs->dobjects[i].gobject_ptr;
				gob->dobject = rs->dobjects + i;
			}
		}
	}
	return g;
}
