#pragma once
#include <stdlib.h>

typedef struct node {
	void* val;
	char* key;
	struct node* next;
} bucket_t;

typedef struct {
	bucket_t* buckets;
	int size;
	int len;
} hash_map_t;

hash_map_t* hash_map_new(int size);
void hash_map_delete(hash_map_t* hash_map);

void* hash_map_get(hash_map_t* hash_map, const char* key);
void hash_map_set(hash_map_t* hash_map, const char* key, void* val);
void hash_map_remove(hash_map_t* hash_map, const char* key);
void hash_map_print_keys(hash_map_t* hash_map);
/* USELESS */
void hash_map_set_data(hash_map_t* hash_map, void* data_vec, size_t elem_size, char* key, void* val);
