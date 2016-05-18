#pragma once

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

void* hash_map_get(hash_map_t* hash_map, char* key);
void hash_map_set(hash_map_t* hash_map, char* key, void* val);
