#ifndef HASH_HASH_H
#define HASH_HASH_H

#include <stdlib.h>
#include <stdio.h>

typedef struct hash_node {
	struct hash_node *next;
	int key;
	void *value;
} hash;

/* This API works with integers only. A custom conversion function for different data is required */

/* Gets a key */
extern void *get(int key);
/* Inserts a key and a value. Collisions will spawn another hash node  */
extern void insert(int key, void *value);
extern void delete(int key);

#endif
