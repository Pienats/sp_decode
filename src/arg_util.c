#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg_util.h"

int build_arg_list(const char* arg_str, int* argc, char*** argv) {
	char* args = NULL;
	char* ptr = NULL;
	int count = 0;
	char** vect = NULL;
	
	if ((arg_str == NULL) || (*arg_str == 0)) {
		printf("Argument string empty or NULL; Aborting\n");
		return -1;
	}
	
	args = calloc(strlen(arg_str)+1, sizeof(char));
	if (args == NULL) {
		printf("Unable to allocate string buffer; Aborting\n");
		return -2;
	}
	
	strcpy(args, arg_str);
	
	// Count the arguments
	count = 1; // Non-NULL string implies at least 1 argument
	for (ptr = args; *ptr != 0; ++ptr) {
		if (*ptr == ' ') {
			count++;
		}
	}
	
	// Allocate argument vector
	vect = calloc(count, sizeof(char*));
	*argc = count;
	
	// Populate the argument vector
	count = 0;
	vect[count++] = args;
	for (ptr = args; *ptr != 0; ++ptr) {
		if (*ptr == ' ') {
			*ptr = 0;
			vect[count++] = ptr + 1;
		}
	}
	
	*argv = vect;
	
	return 0;
}

int free_arg_list(char** argv) {
	free(argv[0]); // Free the actual buffer
	free(argv); // Free the argument vector
	return 0;
}

