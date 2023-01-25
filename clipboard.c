#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;

void handle_input(int argc, char *restrict argv[], char *restrict char_argument, string restrict string_argument, int *restrict int_argument) {
	int i;
	for (i = 0; i < argc; i++) {
		if (!strncmp(argv[i], "-i", 2) || !strncmp(argv[i], "--interface", 11)) {
			 /* string argument */
			if (i < argc-1 && argv[i+1][0] != '-') {
				strncpy(string_argument, argv[i+1], strnlen(argv[i+1], 16));
			} else {
				fputs("Could not find valid interface, make sure it doesn't start with a dash\n", stderr);
				exit(1);
			}
		} else if (!strncmp(argv[i], "-n", 2) || !strncmp(argv[i], "--number", 8)) {
			/* int argument */
			/* this is where it gets a little more complicated, we have to do some trickery */
			if (i < argc-1 && argv[i+1][0] != '-') {
				int_argument[0] = strtol(argv[i+1], NULL, 10);
			} else {
				fputs("Could not find valid count, make sure it doesn't start with a dash\n", stderr);
				exit(1);
			}
		} else if (!strncmp(argv[i], "-m", 2) || !strncmp(argv[i], "--mode", 6)) {
			/* char argument */
			if (i < argc-1 && argv[i+1][0] != '-') {
				strncpy(char_argument, argv[i+1], i);
			} else {
				fputs("Could not find valid mode, make sure it doesn't start with a dash\n", stderr);
				exit(1);
			}
		}
	}

	if (0 == char_argument[0]) {
		char_argument[0] = 'F';  // or what the hell ever the default should be
	}

	if (0 == string_argument[0]) {
		strncpy(string_argument, "eth0", 4);
	}

	if (0 == int_argument[0]) {
		int_argument[0] = 1;
	}
}

int main(int argc, char *argv[]) {
	/*
	 * Requirements:
	 * - At least one string, one char and one number (int)
	 * - The string argument has a non defined length
	 * - Arguments appear in random order
	 * - Arguments are being signaled with a dash (for example: "-i" or "--interface")
	 * - Error handling for when an argument is missing
	 * - Default values for non-existing arguments
	 * - Optionally, escape the quotes around a string, so it can safely start with a dash
	 * - If possible, optimize, optimize, optimize!
	 * */

	if (argc < 2) {
		puts("Usage: ./main"
			 "\n\t-i/--interface:\t\tSpecifies the network interface to use (Optional, default: \"eth0\""
			 "\n\t-n/--number:\t\tSpecifies the amount of packages being sent (Optional, default: 1)"
			 "\n\t-m/--mode:\t\tSpecifies the mode in which to send the package (Optional, default F)");
		return 0;
	}

	int *int_argument = (int *) calloc(1, sizeof(*int_argument));
	if (NULL == int_argument) {
		fputs("Failed to initialize counter container\n", stderr);
		exit(1);
	}
	int_argument[0] = 0;

	char *char_argument = (char *) calloc(1, sizeof(*char_argument));
	if (NULL == char_argument) {
		fputs("Failed to initialize mode container\n", stderr);
		exit(1);
	}
	char_argument[0] = 0x0;

	string string_argument = (string) calloc(4, sizeof(*string_argument));
	if (NULL == string_argument) {
		fputs("Failed to initialize interface container\n", stderr);
		exit(1);
	}
	string_argument[0] = 0x0;

	handle_input(argc, argv, char_argument, string_argument, int_argument);

	printf("Sending %d package(s) to interface \"%s\" with mode '%c'\n", int_argument[0], string_argument, char_argument[0]);

	return 0;
}
