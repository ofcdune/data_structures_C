#ifndef HASHMAP_HASHMAP_H
#define HASHMAP_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "alloc_s.h"

struct hash_root {
    unsigned int (*hashfktn)(unsigned int, unsigned int);
    unsigned int bucket_size;
    struct hash_node **bucket;
};

struct hash_node {
    unsigned int key;

    void *value;
    struct hash_node *next;
};

struct hash_root *hash_initialize(unsigned int bucket_size);
bool hash_insert(struct hash_root *restrict hashmap, unsigned int key, void *restrict value);
void *hash_get(struct hash_root *restrict hashmap, unsigned int key);
bool hash_exists(struct hash_root *restrict hashmap, unsigned int key);
void internal_hash_delete(struct hash_node *restrict node_before);
void *hash_delete(struct hash_root *restrict hashmap, unsigned int key);
void hash_set_hashfunction(struct hash_root *restrict hashmap, unsigned int (*hashfktn)(unsigned int, unsigned int));

#endif
