#include "stack.h"

typedef struct StackElement {
    struct StackElement *next;
    void *element;
} stack_inner;

stack *initialize() {
    stack *aux_node = (stack *) malloc(sizeof(*aux_node));
    stack_inner *stack_end = NULL;
    if (NULL == aux_node) {
        fputs("Could not allocate memory to >>stack *aux_node<<", stderr);
        exit(-1);
    }
    aux_node->size = 0;
    aux_node->stack_end = stack_end;
    return aux_node;
}

void push(void *to_push, stack *stack) {
    if (NULL == stack->stack_end) {
        stack->stack_end = (stack_inner *) malloc(sizeof(*stack->stack_end));
        if (NULL == stack->stack_end) {
            fputs("Could not allocate memory for the stack end", stderr);
            exit(-1);
        }
        stack->stack_end->next = NULL;
        stack->stack_end->element = to_push;
    } else {
        stack_inner *ptr = (stack_inner *) malloc(sizeof(*ptr));
        if (NULL == ptr) {
            fputs("Could not allocate memory for the stack walker", stderr);
            exit(-1);
        }
        ptr->element = to_push;
        ptr->next = stack->stack_end;
        stack->stack_end = ptr;
    }
    stack->size++;
}

void *pop(stack *stack) {
    if (NULL == stack->stack_end) {
        puts("The stack is NULL");
        /* this means the stack has already been purged */
        return NULL;
    } else {
        void *to_return = stack->stack_end->element;
        stack_inner *ptr = stack->stack_end;
        stack->stack_end = stack->stack_end->next;
        stack->size--;
        ptr->next = NULL;
        free(ptr);
        return to_return;
    }
}

void *peek_stack(stack *stack) {
    if (NULL == stack->stack_end) {
        return NULL;
    } else {
        return stack->stack_end->element;
    }
}

char is_empty_stack(stack *stack) {
    if (NULL == stack->stack_end) {
        return 1;
    } else {
        return 0;
    }
}
