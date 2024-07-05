#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include "alloc_s.h"

typedef struct AuxiliaryNodeQ {
    struct QueueElement *queue_start, *queue_end;
} queue;

void queue_enqueue(unsigned long long to_enqueue, queue *queue);
unsigned long long queue_dequeue(queue *queue);
bool queue_is_empty(queue *queue);
queue *queue_initialize();

#endif
