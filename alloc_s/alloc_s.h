#ifndef ALLOC_ALLOC_H
#define ALLOC_ALLOC_H

#include <stdlib.h>
#include <stdio.h>

/* The names of the functions follow the naming conventions of printf_s, strncpy_s, etc. */

void *malloc_s(size_t size, char *ErrorMessage) {
	void *mem = malloc(size);
	if (NULL == mem) {
		fprintf(stderr,"Fatal error: %s", ErrorMessage);
		exit(-1);
	}
	return mem;
}

void *calloc_s(size_t NumOfElements, size_t SizeOfElements, char *ErrorMessage) {
	void *mem = calloc(NumOfElements, SizeOfElements);
	if (NULL == mem) {
		fprintf(stderr,"Fatal error: %s", ErrorMessage);
		exit(-1);
	}
	return mem;
}

void *realloc_s(void *memory, size_t NewSize, char *ErrorMessage) {
	void *mem = realloc(memory, NewSize);
	if (NULL == mem) {
		fprintf(stderr,"Fatal error: %s", ErrorMessage);
		exit(-1);
	}
	return mem;
}

#endif
