#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <stdlib.h>
#include <stdio.h>

#include "alloc_s.h"

typedef struct AuxiliaryNodeS {
    struct StackElement *stack_end;
} stack;

extern void push(stack *stack, void *to_push);
extern void *pop(stack *stack);
extern void *peek_stack(stack *stack);
extern char is_empty_stack(stack *stack);
extern stack *initialize_stack();

#endif