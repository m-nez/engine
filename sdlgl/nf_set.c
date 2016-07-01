#include "nf_set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void nf_set_init(nf_set_t* nf_set, int size, int elem_size) {
	nf_set->data = malloc(elem_size * size);
	nf_set->size = size;
	nf_set->elem_size = elem_size;
	nf_set->len = 0;
}

nf_set_t* nf_set_new(int size, int elem_size) {
	nf_set_t* nf_set = malloc(sizeof(nf_set_t));
	nf_set_init(nf_set, size, elem_size);
	return nf_set;
}

void nf_set_delete(nf_set_t* nf_set) {
	free(nf_set->data);
	free(nf_set);
}

int nf_set_add(nf_set_t* nf_set) {
	if (nf_set->size == nf_set->len) {
		nf_set->size *= 2;
		nf_set->data = realloc(nf_set->data,
				nf_set->size * nf_set->elem_size);
		nf_set->len++;
		return 0;
	}
	nf_set->len++;
	return -1;
}

void* nf_set_get_at(nf_set_t* nf_set, int index) {
	return ((char*)nf_set->data) +
		index * nf_set->elem_size;
}

void* nf_set_get_last(nf_set_t* nf_set) {
	return ((char*)nf_set->data) +
		(nf_set->len - 1) * nf_set->elem_size;
}

int nf_set_remove_at(nf_set_t* nf_set, int index) {
	if (index == nf_set->len - 1) {
		nf_set->len--;
		return -1;
	}
	memcpy( (char*)nf_set->data + (index) * nf_set->elem_size,
			(char*)nf_set->data + (nf_set->len - 1) * nf_set->elem_size,
			nf_set->elem_size);
	nf_set->len--;
	return 0;
}

void nf_set_print(nf_set_t* nf_set) {
	int i, j;
	printf("nf_set: size=%d len=%d elem_size=%d data={\n", nf_set->size, nf_set->len, nf_set->elem_size);
	for (i = 0; i < nf_set->len; ++i) {
		printf("[%d] ", i);
		for (j = 0; j < nf_set->elem_size; ++j) {
			printf("%02X ", (unsigned char)((char*)nf_set->data)[i*nf_set->elem_size + j]);
		}
		printf("\n");
	}
	printf("}\n");
}
