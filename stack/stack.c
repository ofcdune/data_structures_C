#include "stack.h"

stack *stack_end;

void push(void *to_push) {
	if (NULL == stack_end) {
		stack_end = (stack *) malloc(sizeof(* stack_end));
		if (NULL == stack_end) {
			fputs("Could not allocate memory for the stack end", stderr);
			exit(-1);
		}
		stack_end->next = NULL;
		stack_end->element = to_push;
	} else {
		stack *ptr = (stack *) malloc(sizeof(*ptr));
		if (NULL == ptr) {
			fputs("Could not allocate memory for the stack walker", stderr);
			exit(-1);
		}
		ptr->element = to_push;
		ptr->next = stack_end;
		stack_end = ptr;
	}
}

void *pop() {
	if (NULL == stack_end) {
		puts("The stack is NULL");
		/* this means the stack has already been purged */
		return NULL;
	} else {
		void *to_return = stack_end->element;
		stack *ptr = stack_end;
		stack_end = stack_end->next;
		ptr->next = NULL;
		free(ptr);
		return to_return;
	}
}

void *peek() {
	if (NULL == stack_end) {
		return NULL;
	} else {
		return stack_end->element;
	}
}

char is_empty() {
	if (NULL == stack_end) {
		return 1;
	} else {
		return 0;
	}
}
