#include <stdio.h>
#include <stdint.h>

#include "cli.h"

#define HEX_DUMP_CHARS	16


char to_printable(char in_char) {
	if ((in_char < 0x21) || (in_char > 0x7E)) {
		return '.';
	}
	return in_char;
}

void print_hex(const void* data, size_t len) {
	const uint8_t* _data = data;

	char print_string[HEX_DUMP_CHARS + 1] ;
	int print_idx = 0;

	print_string[HEX_DUMP_CHARS] = '\0';
	int idx;

	for (idx = 0; idx < len; ++idx) {
		if (!(idx%HEX_DUMP_CHARS)) {
			if (idx) {
				printf("\t%s\n", print_string);
				print_idx = 0;
				print_string[print_idx] = '\0';
			}
			printf("0x%08X: ", idx);
		}

		// Print a single byte
		printf("%02X ", _data[idx]);
		print_string[print_idx++] = to_printable(_data[idx]);
		print_string[print_idx] = '\0';
	}

	// Determine padding: 10 characters for address, 2 until first byte,
	// 3 per byte printed already, up to 61 chars total
	int padding = idx % 16;
	if (padding) {
		padding *= 3; // 3 chars per byte
		padding += 2; // ": " between address and bytes
		padding += 10; // address
		for (; padding < 61; padding++) {
			printf(" ");
		}
	}
	printf("\t%s", print_string);
	printf("\n");
}

int cli_struct_valid(struct cli_data_t* cli_data) {
	if (cli_data) {
		if (cli_data->ptr_this_struct_9E4 == cli_data) {
			return 1;
		}
	}

	return 0;
}


