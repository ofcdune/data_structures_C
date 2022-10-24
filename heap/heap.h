#ifndef HEAP_HEAP_H
#define HEAP_HEAP_H

#include <stdlib.h>
#include <stdio.h>

typedef struct heap_element {
	void *element;
	int priority;
} heap;

extern void insert(void *to_insert, int priority);
extern void *get_next();
extern void *pop_next();
extern void update_priority(void *to_update, int new_priority);
extern char is_empty();

#endif
