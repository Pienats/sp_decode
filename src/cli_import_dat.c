#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "cli.h"
#include "cli_import_dat.h"
#include "cli_file_util.h"
#include "cli_errors.h"


int read_import_dat(struct cli_data_t* cli_ptr_a, struct aa_t** struct_ptr_offs_002, struct cli_data_t* cli_ptr_b) {
	int r;
	uint32_t import_dat_version;
	uint16_t error = 0;
	size_t size;

	uint16_t abs_size_A_00C;
	uint16_t abs_size_B_010;

	uint32_t file_offset;

//	uint16_t var_6;
//	uint16_t var_8;
//	struct aa_t* var_ptr;

	if (cli_ptr_b->file_import_dat_704) {
		printf("File appears to already be open, closing\n");
		fclose(cli_ptr_b->file_import_dat_704);
		cli_ptr_b->file_import_dat_704 = NULL;
	}

	cli_ptr_b->file_import_dat_704 = fopen(cli_ptr_b->path_to_import_dat_304, "rb");
	if (cli_ptr_b->file_import_dat_704 == NULL) {
		printf("Error opening file %s\n", cli_ptr_b->path_to_import_dat_304);
		error = 1;
		goto exit;
	}

	if (fread_swap_bytes(&import_dat_version, 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading file %s\n", cli_ptr_b->path_to_import_dat_304);
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	printf("import.dat version: 0x%08X\n", import_dat_version);
	if (!((import_dat_version == 5) || (import_dat_version == 6) || (import_dat_version == 7) || (import_dat_version == 8))) {
		error = ERR_0x3A_VERSION;
		goto exit;
	}

	cli_ptr_b->import_dat_version_9C0 = import_dat_version;

	// Make sure the "group of seven" is not yet allocated
	if (cli_ptr_a->ptr_struct_import_data_000 != NULL) {
		printf("cli_ptr_a->ptr_struct_import_data_000 not NULL, freeing\n");
		free(cli_ptr_a->ptr_struct_import_data_000);
		cli_ptr_a->ptr_struct_import_data_000 = NULL;
	}

	// Allocate group of seven
	cli_ptr_a->ptr_struct_import_data_000 = calloc(1, sizeof(struct import_data_t)); // size should be 0x1C
	if (cli_ptr_a->ptr_struct_import_data_000 == NULL) {
		printf("Unable to allocate ptr_struct_import_data_000 (\"group of seven\") memory\n");
		error = ERR_0x08_INSUFFICIENT_MEMORY;
		goto exit;
	}

	if (import_dat_version < 6) {
		printf("Code for import.dat version before ver 6 not implemented [2]\n");
		error = -10;
		goto exit;
	}

	//////////////////////////////////////////////////////
	// Seek file  to "group of seven"
	if (fseek(cli_ptr_b->file_import_dat_704, 8, SEEK_CUR)) {
		printf("Error seeking into file [01]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		// asm doesn't appear to break out to exit here...
	}

	if (fread_swap_bytes(cli_ptr_a->ptr_struct_import_data_000, 1, sizeof(struct import_data_t), cli_ptr_b->file_import_dat_704, 4, 1) != sizeof(struct import_data_t)) {
		printf("Error reading from file [01]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		// asm doesn't appear to break out to exit here...
	}

	if (import_dat_version < 6) {
		printf("Code for import.dat version before ver 6 not implemented [2]\n");
		error = -10;
		goto exit;
	}

	cli_ptr_b->var_long_offset_9C4 = (cli_ptr_a->ptr_struct_import_data_000)->var_long_offset_014;

	//////////////////////////////////////////////////////
	// seek to next data
	if (fseek(cli_ptr_b->file_import_dat_704, 4, SEEK_CUR)) {
		printf("Error seeking into file [02]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_9CA), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [02]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_9CE), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [03]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	// The following seems to overwrite 2 previous values
	if (fread_swap_bytes(&(cli_ptr_a->ptr_struct_import_data_000->var_long_offset_014), 1, 8, cli_ptr_b->file_import_dat_704, 4, 1) != 8) {
		printf("Error reading from file [04]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		// asm doesn't appear to break out to exit here...
		//goto exit;
	}

	if (import_dat_version < 7) {
		printf("Code for import.dat version before ver 7 not implemented [01]\n");
		error = -10;
		goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_9E6), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [05]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		// asm doesn't appear to break out to exit here...
		//goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_9F2), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [06]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		// asm doesn't appear to break out to exit here...
		//goto exit;
	}

	if (fseek(cli_ptr_b->file_import_dat_704, 256, SEEK_CUR)) {
		printf("Error seeking into file [03]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_file_offset_9EA), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [07]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	if (import_dat_version < 8) {
		printf("Code for import.dat version before ver 8 not implemented [01]\n");
		error = -10;
		goto exit;
	}

	///////////////////////////
	// loc_1F44B (seg009:046B) -> just to keep track of where we are at this point
	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_A00), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [08]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		// asm doesn't appear to break out to exit here...
		//goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_A04), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [09]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		// asm doesn't appear to break out to exit here...
		//goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_A08), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [10]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_A0C), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [11]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	if (fread_swap_bytes(&(cli_ptr_b->var_long_offset_A10), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
		printf("Error reading from file [12]\n");
		error = ERR_0x02_BAD_LOCK_FILE_HEADER;
		goto exit;
	}

	if (error) {
		free(cli_ptr_a->ptr_struct_import_data_000);
		cli_ptr_a->ptr_struct_import_data_000 = NULL;
		goto exit;
	}

	///////////////////////////
	// loc_1F551 (seg009:0571) -> just to keep track of where we are at this point
	if (*struct_ptr_offs_002 != NULL) {
		printf("*struct_ptr_offs_002 not NULL, freeing\n");
		free(*struct_ptr_offs_002);
		*struct_ptr_offs_002 = NULL;
	}

	*struct_ptr_offs_002 = calloc(cli_ptr_a->ptr_struct_import_data_000->block_count_offset_018  , sizeof(struct aa_t));
	if (*struct_ptr_offs_002 == NULL) {
		printf("Unable to allocate memory for *struct_ptr_offs_002\n");
		error = ERR_0x08_INSUFFICIENT_MEMORY;
		goto exit;
	}

	///////////////////////////
	// loc_1F591 (seg009:05B1) -> just to keep track of where we are at this point
//	var_6 = 0;
//	var_8 = 0;
	uint32_t loop_count = 0;
	struct aa_t* var_ptr = *struct_ptr_offs_002;

	while (!(error) && (cli_ptr_a->ptr_struct_import_data_000->block_count_offset_018 > loop_count)) {
		// TODO: condense into a single read
		if (fread_swap_bytes(&(var_ptr->var_long_offset_000), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
			printf("Error reading from file [13]\n");
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		if (fread_swap_bytes(&(var_ptr->var_long_offset_004), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
			printf("Error reading from file [13]\n");
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		if (fread_swap_bytes(&(var_ptr->var_long_offset_008), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
			printf("Error reading from file [13]\n");
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		if (fread_swap_bytes(&(var_ptr->var_long_offset_00C), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
			printf("Error reading from file [13]\n");
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		if (fread_swap_bytes(&(var_ptr->var_long_offset_010), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
			printf("Error reading from file [13]\n");
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		if (fread_swap_bytes(&(var_ptr->var_long_offset_014), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
			printf("Error reading from file [13]\n");
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		if (fread_swap_bytes(&(var_ptr->var_long_offset_018), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
			printf("Error reading from file [13]\n");
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		if (import_dat_version >= 6) {
			if (fread_swap_bytes(&(var_ptr->var_long_offset_01C), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1) != 4) {
				printf("Error reading from file [14]\n");
				error = ERR_0x02_BAD_LOCK_FILE_HEADER;
				break;
			}
		}

		if (import_dat_version >= 7) {
			r = fread_swap_bytes(&(var_ptr->var_long_offset_140), 1, 4, cli_ptr_b->file_import_dat_704, 4, 1);
			if (r != 4) {
				printf("Error reading from file [15]; r=%d\n", r);
				error = ERR_0x02_BAD_LOCK_FILE_HEADER;
				// asm does not break here yet
				break;
			}
		}

		r = fread_swap_bytes(var_ptr->array_offset_020, 1, 32, cli_ptr_b->file_import_dat_704, 1, 0);
		if (r != 32) {
			printf("Error reading from file [16]; r=%d\n", r);
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			break;
		}

		r = fread_swap_bytes(var_ptr->array_offset_040, 1, 256, cli_ptr_b->file_import_dat_704, 1, 0);
		if (r != 256) {
			printf("Error reading from file [17]; r=%d\n", r);
			error = ERR_0x02_BAD_LOCK_FILE_HEADER;
			// asm does not break here yet
			break;
		}

		loop_count++;
		var_ptr++;
	}

	if (error) {
		free(*struct_ptr_offs_002);
		*struct_ptr_offs_002 = NULL;
		goto exit;
	}

	if (import_dat_version >= 7) {
		// set009:0874
		size = (cli_ptr_a->ptr_struct_import_data_000->block_count_offset_018/8) + 1;

		cli_ptr_b->ptr_offset_9EE = calloc(cli_ptr_a->ptr_struct_import_data_000->var_long_count_B_010, size);
		if (cli_ptr_b->ptr_offset_9EE == NULL) {
			printf("Unable to allocate memory for cli_ptr_b->ptr_offset_9EE\n");
			free(*struct_ptr_offs_002);
			*struct_ptr_offs_002 = NULL;
			error = 8;
			// goto exit; // asm doesn't break out here
		}

		cli_ptr_b->ptr_offset_9F0 = calloc(cli_ptr_a->ptr_struct_import_data_000->var_long_count_A_00C, size);
		if (cli_ptr_b->ptr_offset_9F0 == NULL) {
			printf("Unable to allocate memory for cli_ptr_b->ptr_offset_9F0\n");
			free(*struct_ptr_offs_002);
			*struct_ptr_offs_002 = NULL;

			if (cli_ptr_b->ptr_offset_9EE) { // asm doesn't test, it just frees (shouldn't need to free though)
				free(cli_ptr_b->ptr_offset_9EE);
				cli_ptr_b->ptr_offset_9EE = NULL;
			}
			error = 8;
			// goto exit; // asm doesn't break out here
		}

		///////////////////////
		// seg009:0910
		abs_size_B_010 = size * cli_ptr_a->ptr_struct_import_data_000->var_long_count_B_010;
		abs_size_A_00C = size * cli_ptr_a->ptr_struct_import_data_000->var_long_count_A_00C;

		file_offset = ftell(cli_ptr_b->file_import_dat_704);

		if (fseek(cli_ptr_b->file_import_dat_704, cli_ptr_b->var_long_file_offset_9EA, SEEK_SET)) {
			error = 2;
		} else {
			r = fread_swap_bytes(cli_ptr_b->ptr_offset_9EE, abs_size_B_010, 1, cli_ptr_b->file_import_dat_704, 1, 0);
			if (r != 1) {
				printf("Error reading %d bytes from file [18]; r=%d\n", abs_size_B_010, r);
				error = 2;
			} else {
				r = fread_swap_bytes(cli_ptr_b->ptr_offset_9F0, abs_size_A_00C, 1, cli_ptr_b->file_import_dat_704, 1, 0);
				if (r != 1) {
					printf("Error reading %d bytes from file [19]; r=%d\n", abs_size_A_00C, r);
					error = 2;
				}
			}
		}

		// Restore file location
		fseek(cli_ptr_b->file_import_dat_704, file_offset, SEEK_SET);
	}

exit:
	return error;
}

