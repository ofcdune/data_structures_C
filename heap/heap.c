#include "heap.h"

heap **heap_array;
int size;

/* Compares the given node with its father node in the heap without recursion */
void bubble_up(int);
/* Compares the given node with its children nodes in the heap without recursion */
void sink_down(int);
/* Swaps two values at given indices */
void swap(int, int);

void swap(int to_swap, int with) {
	heap *temp_heap = heap_array[to_swap];
	heap_array[to_swap] = heap_array[with];
	heap_array[with] = temp_heap;
}

void bubble_up(int index) {
	int half = index / 2;
	while (index > 1 && heap_array[half]->priority < heap_array[index]->priority) {
		swap(half, index);
		index = half;
		half = index / 2;
	}
}

void sink_down(int index) {
	int left = index * 2;
	int right = left + 1;

	while (left <= size) {
		if (right <= size) {
			if (heap_array[left]->priority >= heap_array[right]->priority) {
				if (heap_array[left]->priority > heap_array[index]->priority) {
					swap(left, index);
					index = left;
				} else {
					break;
				}
			} else {
				if (heap_array[right]->priority > heap_array[index]->priority) {
					swap(right, index);
					index = right;
				} else {
					break;
				}
			}
		} else {
			if (heap_array[left]->priority > heap_array[index]->priority) {
				swap(left, index);
				index = left;
			} else {
				break;
			}
		}
		left = index * 2;
		right = left + 1;
	}
}

void insert(void *to_insert, int priority) {
	if (NULL == heap_array) {
		heap_array = (heap **) calloc(2, sizeof(**heap_array));
		if (NULL == heap_array) {
			fputs("Failed to initialize heap", stderr);
			exit(-1);
		}
		heap_array[0] = (heap *) calloc(1, sizeof(*heap_array[0]));
		heap_array[1] = (heap *) calloc(1, sizeof(*heap_array[1]));
		if (NULL == heap_array[0] || NULL == heap_array[1]) {
			fputs("Failed to initialize heap", stderr);
			exit(-1);
		}
		heap_array[0] = NULL;
		heap_array[1]->element = to_insert;
		heap_array[1]->priority = priority;
		size = 1;
	} else {
		heap **temp;
		temp = (heap **) realloc(heap_array, sizeof(**heap_array)*++size);
		if (NULL == temp) {
			fputs("Failed to grow heap", stderr);
			exit(-1);
		}
		temp[size] = (heap *) calloc(1, sizeof(*temp[size]));
		temp[size]->element = to_insert;
		temp[size]->priority = priority;
		heap_array = temp;
		bubble_up(size);
	}
}

void *pop_next() {
	if (NULL == heap_array) {
		return NULL;
	} else {
		void *to_return = heap_array[1]->element;
		swap(size, 1);
		heap **temp;
		temp = (heap **) realloc(heap_array, sizeof(**heap_array)*--size);
		if (NULL == temp) {
			fputs("Failed to shrink heap", stderr);
			exit(-1);
		}
		heap_array = temp;
		if (size > 1) {
			sink_down(1);
		}
		return to_return;
	}
}

void update_priority(void *to_update, int new_priority) {
	int i, old_priority;
	for (i = 1; i <= size; i++) {
		if (heap_array[i]->element == to_update) {
			old_priority = heap_array[i]->priority;
			if (new_priority > old_priority) {
				heap_array[i]->priority = new_priority;
				bubble_up(i);
			} else if (new_priority == old_priority) {
				return;
			} else {
				heap_array[i]->priority = new_priority;
				sink_down(i);
			}
			return;
		}
	}
}

void *peek() {
	if (NULL == heap_array) {
		return NULL;
	} else {
		return heap_array[1]->element;
	}
}

char is_emtpy() {
	if (NULL == heap_array) {
		return 1;
	} else {
		return 0;
	}
}
