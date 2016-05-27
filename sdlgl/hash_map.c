#include "hash_map.h"
#include <string.h>
#include <stdio.h>

hash_map_t* hash_map_new(int size) {
	int i;

	hash_map_t* hash_map = malloc(sizeof(hash_map_t));
	hash_map->size = size;
	hash_map->len = 0;
	hash_map->buckets = malloc(sizeof(bucket_t) * hash_map->size);
	for(i = 0; i < hash_map->size; ++i) {
		hash_map->buckets[i].key = NULL;
		hash_map->buckets[i].val = NULL;
		hash_map->buckets[i].next = NULL;
	}

	return hash_map;
}

void hash_map_delete(hash_map_t* hash_map) {
	int i;
	bucket_t* bucket;
	bucket_t* freebucket;
	for(i = 0; i < hash_map->size; ++i) {
		bucket = hash_map->buckets + i;
		if (bucket->key == NULL) {
			continue;
		}
		free(bucket->key);
		bucket = bucket->next;
		while (bucket != NULL) {
				freebucket = bucket;
				free(bucket->key);
				bucket = bucket->next;
				free(freebucket);
		}
	}
	free(hash_map->buckets);
	free(hash_map);
}

static int hash_map_function(hash_map_t* hash_map, const char* key) {
	int i;
	int hash = 21589731;
	for(i = 0; key[i] != 0; ++i) {
		hash += key[i] * ( 17 + i * 2 );
	}
	hash %= hash_map->size;
	return hash;
}

void* hash_map_get(hash_map_t* hash_map, char* key) {
	int index = hash_map_function(hash_map, key);
	bucket_t* bucket = hash_map->buckets + index;

	if (bucket->next == NULL) {
		return bucket->val;
	} else {
		do {
			if (strcmp(key, bucket->key) == 0) {
				return bucket->val;
			}
			bucket = bucket->next;
		} while (bucket != NULL);
	}
	return NULL;
}

void hash_map_set(hash_map_t* hash_map, const char* key, void* val) {
	int index = hash_map_function(hash_map, key);
	bucket_t* bucket = hash_map->buckets + index;
	bucket_t** b_next = &bucket;

	if (bucket->key != NULL) {
		do {
			if (strcmp(key, bucket->key) == 0) {
				bucket->val = val;
				return;
			}
			b_next = &bucket->next;
			bucket = bucket->next;
		} while (bucket != NULL);
		*b_next = malloc(sizeof(bucket_t));
	}
	(*b_next)->val = val;
	(*b_next)->key = malloc(strlen(key) + 1);
	strcpy((*b_next)->key, key);
	(*b_next)->next = NULL;
	hash_map->len++;
}

void hash_map_remove(hash_map_t* hash_map, char* key) {
	int index = hash_map_function(hash_map, key);
	bucket_t* bucket = hash_map->buckets + index;
	bucket_t* prev = NULL;

	if (bucket->key != NULL) {
		if (bucket->next != NULL) {
			bucket->val = NULL;
			free(bucket->key);
			bucket->key = NULL;
			hash_map->len--;
		} else {
			do {
				if (strcmp(key, bucket->key) == 0) {
					if (prev == NULL) {
						bucket->val = NULL;
						free(bucket->key);
						bucket->key = NULL;
						hash_map->len--;
					} else {
						prev->next = bucket->next;
						free(bucket->key);
						free(bucket);
						hash_map->len--;
					}
				}
				prev = bucket;
				bucket = bucket->next;
			} while (bucket != NULL);
		}
	}
}
void hash_map_print_keys(hash_map_t* hash_map) {
	int i;
	bucket_t* bucket;
	for(i = 0; i < hash_map->size; ++i) {
		if (hash_map->buckets[i].key != NULL)	{
			bucket = hash_map->buckets + i;
			do {
				printf("%s\n", bucket->key);
				bucket = bucket->next;
			} while(bucket != NULL);
		}
	}
}

void hash_map_set_data(hash_map_t* hash_map, void* data_vec, size_t elem_size, char* key, void* val) {
	char* d = (char*) data_vec;
	memcpy(d + elem_size * hash_map->len, val, elem_size);
	hash_map_set(hash_map, key, d + elem_size * hash_map->len);
}
