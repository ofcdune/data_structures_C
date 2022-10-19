#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct stack_element {
	struct stack_element *next;
	void *element;
} stack;

#endif
