#include "stack.h"

typedef struct StackElement {
    struct StackElement *next;
    void *element;
} stack_inner;

stack *initialize_stack() {
    NEW(stack, aux_node, 1)

    stack_inner *stack_end = NULL;
    aux_node->stack_end = stack_end;
    return aux_node;
}

void push(stack *stack, void *to_push) {
    if (NULL == stack->stack_end) {
        stack->stack_end = (stack_inner *) calloc(1, sizeof(*stack->stack_end));
        VALNUL(stack->stack_end)

        stack->stack_end->next = NULL;
        stack->stack_end->element = to_push;
    } else {
        NEW(stack_inner, stack_walker, 1)

        stack_walker->element = to_push;
        stack_walker->next = stack->stack_end;
        stack->stack_end = stack_walker;
    }
}

void *pop(stack *stack) {
    if (NULL == stack->stack_end) {
        /* this means the stack has already been purged */
        return NULL;
    } else {
        void *to_return = stack->stack_end->element;
        stack_inner *ptr = stack->stack_end;
        stack->stack_end = stack->stack_end->next;
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