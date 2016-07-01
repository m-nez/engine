#include "gobjects.h"
#include <stdio.h>

gobjects_t* gobjects_new(size_t size, render_states_t* render_states) {
	gobjects_t* g = malloc(sizeof(gobjects_t));
	g->map = hash_map_new(size);
	g->data = malloc(sizeof(gobject_t) * size);
	g->render_states = render_states;
	g->col_world = col_world_new();
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
	if (gp->dobject != NULL) {
		dobject_t* d = gp->dobject;
		int dobject_index = d - gp->render_state->dobjects;
		if (render_state_remove_at(gp->render_state, dobject_index) != -1) {
			/* Swap has occured, so dobject pointer needs fixing */
			if (d->gobject_ptr != NULL) { /* d is now the swapped dobject from the back of the array */
				/* swapped dobject is referenced by a gobject */
				gobject_t* g_to_update = (gobject_t*)d->gobject_ptr;
				g_to_update->dobject = d;
			}
		}

	}
	if (gp->col_object != NULL) {
		col_object_t* col_object = gp->col_object;
		int col_object_index = nf_set_get_index(gobjects->col_world->col_sets + gp->col_type, col_object);
		if (nf_set_remove_at(gobjects->col_world->col_sets + gp->col_type, col_object_index) != -1) {
			/* Swap has occured, so col_object pointer needs fixing */
			gobject_t* g_to_update = col_object->user_ptr;
			g_to_update->col_object = col_object;
			
		}
	}
	if (gp - gobjects->data != gobjects->map->len - 1) {
		/* Not the last element, so we have to swap */
		memcpy(gp, gobjects->data + gobjects->map->len - 1, sizeof(gobject_t));
		/* Update dobject's pointer */
		if (gp->dobject != NULL) {
			gp->dobject->gobject_ptr = gp;
		}
		/* Update col_object's pointer */
		if (gp->col_object != NULL) {
			gp->col_object->user_ptr = gp;
		}
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

gobject_t* gobjects_add_draw_phys(gobjects_t* gobjects, const char* name, const char* rs_name, int col_type) {
	gobject_t* g = gobjects_add_drawable(gobjects, name, rs_name);
	if (g == NULL) {
		return NULL;
	}
	if (col_type > NUM_COL_SHAPES || col_type < 0) {
		fprintf(stderr, "Warning: Invalid col_type \"%d\".\n", col_type);
		return g;
	}
	nf_set_t* col_set = gobjects->col_world->col_sets + col_type;
	void* data_pre =col_set->data;
	nf_set_add(col_set);
	g->col_object = nf_set_get_last(col_set);
	col_shape_init(g->col_object, col_type);
	g->col_type = col_type;
	if (data_pre != col_set->data) {
		/* Adding caused a realloc, gobjects' col_object pointers have to be updated */
		int i;
		for (i = 0; i < col_set->len; ++i) {
			col_object_t* cob = nf_set_at(col_set, i);
			gobject_t* gob = cob->user_ptr;
			gob->col_object = cob;
		}
	}
	return g;
}
