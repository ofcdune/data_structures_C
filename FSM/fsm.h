#ifndef FSM_FSM_H
#define FSM_FSM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void set_pattern(const char *new_pattern, int new_size);
/* Returns the new value for a given pattern, state and letter, assuming that pattern is a global variable */
extern int sigma(int state, char letter);
/* Prints out the match indices and returns amount of matches */
int match(const char *string, int length);

#endif
