#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../alloc_s.h"

typedef struct AuxiliaryNodeQ {
    struct QueueElement *queue_start, *queue_end;
} queue;

void queue_enqueue(void *restrict to_enqueue, queue *restrict queue);
void *queue_dequeue(queue *queue);
bool queue_is_empty(queue *queue);
queue *queue_initialize();

#endif
