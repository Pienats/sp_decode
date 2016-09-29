#include <stdio.h>
#include <stdint.h>

#include "cli_file_util.h"

/**
 * Perform a byte swap in a buffer in groups of a given size
 *
 * @param buffer Buffer in which the bytes should be swapped
 * @param count Number of groups for which data should be swapped
 * @param group_size Size of each group (in bytes)
 */
static void swap_bytes(void* buffer, size_t count, size_t group_size) {
	uint8_t* low_byte;
	uint8_t* high_byte;
	uint8_t tmp_byte;

	while (count) {
		count--;

		low_byte = (buffer + (count * group_size));
		high_byte = low_byte + (group_size - 1);

		while (low_byte < high_byte) {
			tmp_byte = *low_byte;
			*low_byte = *high_byte;
			*high_byte = tmp_byte;

			high_byte--;
			low_byte++;
		}
	}
}

/**
 * Read data from a file, but swap the bytes in groupings as needed
 *
 * @param buffer Buffer to read data into
 * @param size Size of each block
 * @param count Number of blocks to read
 * @param stream Stream to read the data from
 * @param swap_count Number of bytes to swap at a time
 * @param do_swap Indicate whether or not the data should be swap
 *
 * @return The number of blocks of size @size that was read
 */
int fread_swap_bytes(void* buffer, size_t size, size_t count, FILE* stream, size_t group_size, int do_swap) {
	size_t group_count;
	size_t len_read;

	len_read = fread(buffer, size, count, stream);

	group_count = (size * len_read) / group_size;

	if (do_swap) {
		swap_bytes(buffer, group_count, group_size);
	}


	return len_read;
}



