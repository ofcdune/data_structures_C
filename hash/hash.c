#include "hash.h"

/* Feel free to edit the size according to your needs */
#define MAXSIZE 256

hash *h_array[MAXSIZE];

int hash_function(int value) {
	return value % MAXSIZE;
}

void *get(int key) {
	int hash_value = hash_function(key);
	if (NULL == h_array[hash_value]) {
		return NULL;
	} else {
		/* traverse through the chain */
		hash *ptr = h_array[hash_value];
		while (NULL != ptr) {
			if (ptr->key == key) {
				return ptr->value;
			}
			ptr = ptr->next;
		}
		return NULL;
	}
}

void insert(int key, void *value) {
	int hash_value = hash_function(key);
	if (NULL == h_array[hash_value]) {
		h_array[hash_value] = (hash *) calloc(1, sizeof(*h_array[hash_value]));
		if (NULL == h_array[hash_value]) {
			fputs("Failed to allocate bucked", stderr);
			exit(-1);
		}
		h_array[hash_value]->key = key;
		h_array[hash_value]->value = value;
		h_array[hash_value]->next = NULL;
	} else {
		hash *ptr = h_array[hash_value];
		if (ptr->key == key) {
			ptr->value = value;
			return;
		}
		while (NULL != ptr->next) {
			if (ptr->next->key == key) {
				ptr->next->value = value;
				return;
			}
			ptr = ptr->next;
		}
		ptr->next = (hash *) calloc(1, sizeof(*ptr->next));
		if (NULL == h_array[hash_value]) {
			fputs("Failed to allocate bucked", stderr);
			exit(-1);
		}
		ptr->next->key = key;
		ptr->next->value = value;
		ptr->next->next = NULL;
	}
}

void delete(int key) {
	int hash_value = hash_function(key);
	if (NULL != h_array[hash_value]) {
		hash *ptr = h_array[hash_value];
		hash *ptr2;
		if (ptr->key == key) {
			ptr2 = ptr->next;
			free(ptr);
			h_array[hash_value] = ptr2;
			return;
		}
		while (NULL != ptr->next) {
			if (ptr->next->key == key) {
				ptr2 = ptr->next->next;
				free(ptr->next);
				ptr->next = ptr2;
				return;
			}
			ptr = ptr->next;
		}
		return;
	}
}
