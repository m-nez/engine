#pragma once

/* No fragmentation set
 *
 * Structure allowing for iteration over a continous array;
 * During addition and removal of elements
 * their location in the array may change.
 * */

typedef struct {
	void* data;
	int size; /* Capacity of the set in number of elements. If exceded it will be doubled */
	int len; /* Number of elements currently stored */
	int elem_size; /* Size of a single element */
} nf_set_t;

void nf_set_init(nf_set_t* nf_set, int size, int elem_size);

nf_set_t* nf_set_new(int size, int elem_size);

void nf_set_delete(nf_set_t* nf_set);

/* Add an element with undefined contents
 * to the end of the set. If realloc didn't occur
 * return -1. */
int nf_set_add(nf_set_t* nf_set);

void* nf_set_get_at(nf_set_t* nf_set, int index);

void* nf_set_get_last(nf_set_t* nf_set);

/* Remove element at index.
 * if no swap occured return -1. */
int nf_set_remove_at(nf_set_t* nf_set, int index);

void nf_set_print(nf_set_t* nf_set);

static inline void* nf_set_at(nf_set_t* nf_set, int index) {
	return ((char*)nf_set->data) + index * nf_set->elem_size;
}

static inline int nf_set_get_index(nf_set_t* nf_set, void* ptr) {
	return (((char*)ptr) - (char*)nf_set->data) / nf_set->elem_size;
}
