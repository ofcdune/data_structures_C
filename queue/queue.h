#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct queue_element {
	struct queue_element *next;
	void *element;
} queue;

extern void enqueue(void *to_enqueue);
extern void *dequeue();
extern char is_empty();

#endif
