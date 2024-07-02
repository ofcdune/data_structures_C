#ifndef ALLOC_ALLOC_H
#define ALLOC_ALLOC_H

#include <stdio.h>
#include <stdlib.h>

#define VALNUL(x) if (NULL == (x)) {fprintf(stderr, #x " in function %s is NULL, exiting!\n", __func__);exit(1);}
#define NEW(type, name, amount) \
    type *name = (type *) calloc(amount, sizeof(*name)); \
    VALNUL(name)

#endif
