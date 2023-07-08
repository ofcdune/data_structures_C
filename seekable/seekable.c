#include "seekable.h"

seek *initialize() {
    seek *toreturn = (seek *) calloc(1, sizeof(*toreturn));
    if (NULL == toreturn) {
        fputs("Failed to initialize buffer", stderr);
        exit(1);
    }

    toreturn->start = (struct SeekableNode *) calloc(1, sizeof(*toreturn->start));
    if (NULL == toreturn->start) {
        fputs("Failed to initialize buffer", stderr);
        exit(1);
    }
    toreturn->current = toreturn->start;

    /* initializing the sentinel nodes right and left of the current position */
    insert_node_right(toreturn);
    insert_node_left(toreturn);

    return toreturn;
}

void *peek(seek *node) {
    if (NULL != node->current->object) {
        return node->current->object;
    }
    return NULL;
}

void *peek_right(seek *node) {
    if (NULL != node->current->next && NULL != node->current->next->object) {
        return node->current->next->object;
    }
    return NULL;
}

void *peek_left(seek *node) {
    if (NULL != node->current->previous && NULL != node->current->previous->object) {
        return node->current->previous->object;
    }
    return NULL;
}

char seek_right(seek *node) {
    if (NULL != node->current->next) {
        node->current = node->current->next;
        return 0;
    }
    return -1;
}

char seek_left(seek *node) {
    if (NULL != node->current->previous) {
        node->current = node->current->previous;
        return 0;
    }
    return -1;
}

void set(void *restrict object, struct SeekableNode *restrict node) {
    if (NULL != node->object) {
        free(node->object);
    }
    node->object = object;
}

void set_right(void *restrict object, struct SeekableNode *restrict node) {
    if (NULL != node->next) {
        set(object, node->next);
    }
}

void set_left(void *restrict object, struct SeekableNode *restrict node) {
    if (NULL != node->previous) {
        set(object, node->previous);
    }
}

void insert_node_right(seek *node) {
    struct SeekableNode *ptr = calloc(1, sizeof(*ptr));
    if (NULL == ptr) {
        fputs("Failed to initialize buffer", stderr);
        exit(1);
    }

    if (NULL != node->current->next) {
        struct SeekableNode *ptr2 = node->current->next;
        ptr->next = ptr2;
        ptr2->previous = ptr;
    }

    ptr->previous = node->current;
    node->current->next = ptr;
}

void insert_node_left(seek *node) {
    struct SeekableNode *ptr = calloc(1, sizeof(*ptr));
    if (NULL == ptr) {
        fputs("Failed to initialize buffer", stderr);
        exit(1);
    }

    if (NULL != node->current->previous) {
        struct SeekableNode *ptr2 = node->current->previous;
        ptr->previous = ptr2;
        ptr2->next = ptr;
    }

    ptr->next = node->current;
    node->current->previous = ptr;
}
