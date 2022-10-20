#include "queue.h"

queue *queue_start;
queue *queue_end;

void enqueue(void *to_enqueue) {
	if (NULL == queue_end && NULL == queue_start) {
		queue_end = calloc(1, sizeof(*queue_end));
		if (NULL == queue_end) {
			fprintf(stderr, "Failed to initialize queue end");
			exit(-1);
		}
		queue_end->element = to_enqueue;
		queue_start = queue_end;
		queue_end->next = NULL;
	} else {
		/* the edge case here is that the end pointer points to the same element as queue_start */
		queue *ptr = calloc(1, sizeof(*ptr));
		ptr->element = to_enqueue;
		ptr->next = NULL;
		queue_end->next = ptr;
		queue_end = queue_end->next;
	}
}

void *dequeue() {
	if (NULL == queue_end && NULL == queue_start) {
		/* this means the queue has already been purged, we cannot return nothing, so we allocate one byte on the heap */
		return malloc(1);
	} else {
		/* now we need to make sure that if the last element is taken, both queue ends are NULL */
		queue *ptr;
		ptr = queue_start;
		void *to_return = ptr->element;
		if (NULL == queue_start->next) {
			/* meaning, we have only one element in the queue, the queue start is also the queue end */
			queue_start = NULL;
			queue_end = NULL;
		} else {
			queue_start = queue_start->next;
			ptr->next = NULL;
		}
		free(ptr);
		return to_return;
	}
}

char is_empty() {
	if (NULL == queue_end || NULL == queue_start) {
		return 1;
	} else {
		return 0;
	}
}
