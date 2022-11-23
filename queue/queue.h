#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct AuxiliaryNodeQ {
	unsigned int size;
	struct QueueElement *queue_start, *queue_end;
} queue;

extern void enqueue(void *to_enqueue, queue *queue);
extern void *dequeue(queue *queue);
extern void *peek_queue(queue *queue);
extern char is_empty_queue(queue *queue);
extern queue *initialize_queue();

#endif
