#include "fsm.h"

char *pattern;
int **graph;
int p_size;

void build_graph() {
	if (NULL == graph) {
		graph = (int **) calloc(p_size, sizeof(*graph));
		if (NULL == graph) {
			fputs("Failed to create graph", stderr);
			exit(-1);
		}
	} else {
		int **temp = (int **) realloc(graph, sizeof(*graph)*p_size);
		if (NULL == temp) {
			fputs("Failed to reallocate graph", stderr);
			exit(-1);
		}
		graph = temp;
	}

	int i, j;
	for (i = 0; i < p_size; i++) {
		graph[i] = (int *) calloc(95, sizeof(*graph[i]));
		for (j = 0; j < 95; j++) {
			graph[i][j] = sigma(i, j + 32);
			if (0 != graph[i][j]) {
				printf("state %d + '%c' -> %d (new state)\n", i, j + 32, graph[i][j]);
			}
		}
	}
}


int match(const char *string, int length) {
	int i, matches;
	int state = 0;
	matches = 0;
	for (i = 0; i < length; i++) {
		if (string[i] == '\0') {
			continue;
		}
		state = graph[state][string[i] - 32];
		if (state == p_size) {
			printf("Valid shift from %d to %d\n", i + 1 - p_size, i);
			state = 0;
			matches++;
		}
	}
	return matches;
}


void set_pattern(const char *new_pattern, int new_size) {
	new_size++;
	if (NULL == pattern) {
		pattern = (char *) calloc(new_size, sizeof(*pattern));
		if (NULL == pattern) {
			fputs("Failed to set pattern", stderr);
			exit(-1);
		}
	} else {
		char *temp = (char *) realloc(pattern, sizeof(*pattern)*new_size);
		if (NULL == temp) {
			fputs("Failed to re-set pattern", stderr);
			exit(-1);
		}
		pattern = temp;
		memset(pattern, 0, new_size);
	}

	memcpy(pattern, new_pattern, new_size);
	if (NULL == pattern) {
		fputs("Could not set pattern", stderr);
		exit(-1);
	}
	p_size = new_size-1;
	build_graph();
}

int sigma(int state, char letter) {
	int new_state = 0;
	int i, j, k;
	char new_word[state+1];
	for (i = 0; i < state; i++) {
		new_word[i] = pattern[i];
	}
	new_word[state] = letter;

	char pattern_substring[state+1];
	for (i = 1; i < state+2; i++) {
		for (j = 0; j < i; j++) {
			pattern_substring[j] = pattern[j];
		}
		k = 0;
		for (j = 0; j < i; j++) {
			if (pattern_substring[j] == new_word[j+state+1-i]) {
				k++;
			}
		}
		if (k == j) {
			new_state = i;
		}
	}
	return new_state;
}
