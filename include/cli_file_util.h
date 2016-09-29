#ifndef __CLI_FILE_UTIL_H__
#define __CLI_FILE_UTIL_H__

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
int fread_swap_bytes(void* buffer, size_t size, size_t count, FILE* stream, size_t group_size, int do_swap);

#endif // __CLI_FILE_UTIL_H__
