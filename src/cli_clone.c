#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

#include "cli.h"
#include "cli_crypto.h"
#include "cli_file_util.h"
#include "cli_import_dat.h"
#include "cli_prefs_prf.h"

#include "cli_errors.h"
#include "arg_util.h"

#define VENDINIT_DATA_OFFSET 0x4526

static const char* base_path_str = BASE_PATH;
static const char* cli_cmd_line_args = NULL;
static int verbose = 0;
static FILE* vendinit_file = NULL;
static FILE* error_file = NULL;
static FILE* import_file = NULL;
static FILE* prefs_file = NULL;

struct dump_skip_t {
	uint32_t skip_offset;
	uint32_t skip_count;
};

/**
 * Prints program usage
 * @param argc Argument count
 * @param argv Argument vectors
 */
void print_usage(int argc, char** argv) {
	printf("argc=%d\n", argc);
	printf("Usage:\n\t%s [options]\n\n", argv[0]);
	printf("Options:\n");
	printf("\t-a <arg>\t: Arguments that would have been passed to true CLI.EXE\n");
	printf("\t-b <arg>\t: Base path of binary files; include trailing \"/\" (Defaults to the current directory)\n");
	printf("\t-h/-?\t\t: Print this message\n");
	printf("\t-v\t\t: Run in verbose mode\n");
}

/**
 * Parse arguments passed to the program
 * (Note no error checking is done)
 *
 * @param argc Argument count
 * @param argv Argument vectors
 *
 * @return 0 for success, less than zero otherwise
 */
static int parse_args(int argc, char** argv) {
	int r;

	for (int i = 1; i < argc; ) {
		if (argv[i][0] == '-') {
			// Option entry
			switch (argv[i][1]) {
				case 'a' :
					cli_cmd_line_args = argv[++i];
					break;
				case 'b' :
					// Set base path
					base_path_str = argv[++i];
					break;
				case 'h' :
				case '?' :
					// Print help/usage
					print_usage(argc, argv);
					return -2;
				case 'v' :
					// Enable verbosity
					verbose = 1;
					break;
				default :
					printf("Unknown option: %s\n", argv[i]);
					print_usage(argc, argv);
					return -3;
			}
		} else {
			printf("Illegal parameter encountered: %s\n", argv[i]);
			return -4;
		}
		++i;
	}

	return 0;
}

/**
 * Build a full file name based on the base path
 *
 * @param dst_str Pointer to the destination where the full file name is to be stored
 * @param file_name The file name excluding the base path string
 */
void set_file_path(char* fp_str, char* file_name) {
	fp_str = strcpy(fp_str, base_path_str);
	fp_str = strcat(fp_str, "/");
	fp_str = strcat(fp_str, file_name);
}

/**
 * Prints the vend* error associated with the given code
 * @param ln Line number to be printed
 */
void print_error_line(int ln) {
	int to_go = ln;
	char error_str[256];

	// Reset the error file
	fseek(error_file, 0, SEEK_SET);

	while ((to_go) && fgets(error_str, sizeof(error_str), error_file)) {
		--to_go;
	}

	// TODO: not foolproof, test for EOF
	printf("%s\n", error_str);
}

int my_fx_init(void* ptr_c, void* data_to_update) {
	uint8_t vars[4];


	return 0;
}

int alloc_cli_struct(struct cli_data_t** my_struct) {
	// seg012:0000
	uint16_t result = 0;
	struct cli_data_t* local_struct;

	if (*my_struct == NULL) {
		*my_struct = calloc(1, sizeof(struct cli_data_t)); // should ideally be sizeof(struct cli_data_t)
		if (*my_struct) {
			local_struct = *my_struct;

			// optimised version for struct initialization
			memset(local_struct, 0, sizeof(struct cli_data_t));
			local_struct->ptr_this_struct_9E4 = local_struct;
			local_struct->var_offset_816 = 1;
			local_struct->var_offset_9DE = 1;
		} else {
			printf("Unable to allocate memory for my_struct\n");
			return ERR_0x08_INSUFFICIENT_MEMORY;
		}
	} else {
		printf("my_struct != NULL\n");
		return ERR_0x38_BAD_PARAM;
	}

	return result;
}

int free_cli_struct(struct cli_data_t** cli_struct) {
	int ret_val = 0;

	if ((cli_struct) && (*cli_struct)) {
		struct cli_data_t* _cli_struct = *cli_struct;

		if (_cli_struct->ptr_this_struct_9E4 != *cli_struct) {
			printf("free_cli_struct: _cli_struct->ptr_this_struct_9E4 != *cli_struct");
			ret_val = ERR_0x38_BAD_PARAM;
			goto exit;
		}

		printf("WHD: freeing CLI structure and internal data\n");
		// Free sub-pointers first, where applicable
		if (_cli_struct->ptr_struct_aa_002) {
			free(_cli_struct->ptr_struct_aa_002);
			_cli_struct->ptr_struct_aa_002 = NULL;
		}
		if (_cli_struct->ptr_struct_import_data_000) {
			free(_cli_struct->ptr_struct_import_data_000);
			_cli_struct->ptr_struct_import_data_000 = NULL;
		}
		if (_cli_struct->ptr_offset_9EE) {
			free(_cli_struct->ptr_offset_9EE);
			_cli_struct->ptr_offset_9EE = NULL;
		}
		if (_cli_struct->ptr_offset_9F0) {
			free(_cli_struct->ptr_offset_9F0);
			_cli_struct->ptr_offset_9F0 = NULL;
		}
		if (_cli_struct->file_import_dat_704) {
			fclose(_cli_struct->file_import_dat_704);
			_cli_struct->file_import_dat_704 = NULL;
		}

		if (_cli_struct->file_prefs_prf_706) {
			fclose(_cli_struct->file_prefs_prf_706);
			_cli_struct->file_prefs_prf_706 = NULL;
		}

		free(*cli_struct);
		*cli_struct = NULL;
		_cli_struct = NULL;
	} else {
		ret_val = ERR_0x32_GLBL_PTR_NULL;
		goto exit;
	}

exit:
	return ret_val;
}

int my_fx_srand_init(struct main_struct_112_t** ptr_to_main_struct_112) {
	srand(time(NULL));

	// Seems like __harderr is a dos function call to handle int 24.
	// Parameter is the handler function (in this case seg007:0002).
	(*ptr_to_main_struct_112)->srand_val_offset_0 = 0;

	return 0;
}

int my_fx_sub_1CA91(int arg_1, int arg_2) {

}

/**
 * Function to emulate function pointer stored in the CLI structure
 *
 * maybe params should be (uint32_t, char*)
 */
int some_fx(void* arg_1, void* arg_2) {
	void* var_2 = arg_1;

	if ((*(uint16_t*)(var_2 + 3) == 1) || (*(uint16_t*)(var_2 + 3) == 2)) {
		printf("some_fx: %d	%s\n", *(int32_t*)(arg_2+4),(char*)(arg_2+6));	// Original code uses %ld, but that is on a 16 bit machine
	}

	return 0;
}

void dump_struct_data(uint16_t base_addr, const void* dump_struct, size_t data_len, const struct dump_skip_t* skip_info, size_t skip_len) {
	const uint8_t* data = dump_struct;
	char print_string[9];
	print_string[8] = '\0';
	int print_idx = 0;

	uint16_t offset = 0;
	uint32_t idx = 0;

	while (idx < data_len) {
		if (!(offset%16)) {
			if (offset) {
				printf("\t%s\n", print_string);
				print_idx = 0;
				print_string[print_idx] = '\0';
			}
			printf("0x%08X : ", base_addr + offset);
		}

		// Print a single byte
		printf("%02X ", data[idx]);
		print_string[print_idx++] = to_printable(data[idx++]);
		print_string[print_idx] = '\0';
		offset++;

		for (int skip_idx = 0; skip_idx < skip_len; ++skip_idx) {
			if (idx == skip_info[skip_idx].skip_offset) {
				idx += skip_info[skip_idx].skip_count;
				break;
			}
		}
	}

	// Determine padding: 10 characters for address, 3 until first byte,
	// 3 per byte printed already, up to 61 chars total
	int padding = offset % 16;
	if (padding) {
		padding *= 3; // 3 chars per byte
		padding += 3; // " : " between address and bytes
		padding += 10; // address
		for (; padding < 61; padding++) {
			printf(" ");
		}
		printf("\t%s", print_string);
	}
	printf("\n");
}

void dump_struct_cli_t(const struct cli_data_t* cli) {
	printf("sizeof(struct cli_data_t)=%zu\n", sizeof(struct cli_data_t));
	printf("cli->ptr_struct_import_data_000: %p\n", cli->ptr_struct_import_data_000);
	printf("cli->ptr_struct_aa_002: %p\n", cli->ptr_struct_aa_002);
	printf("cli->file_import_dat_704: %p\n", cli->file_import_dat_704);
	printf("cli->file_prefs_prf_706: %p\n", cli->file_prefs_prf_706);
	printf("cli->ptr_offset_9B2: %p\n", cli->ptr_offset_9B2);
	printf("cli->ptr_fx_9AE: %p\n", cli->ptr_fx_9AE);
	printf("cli->ptr_this_struct_9E4: %p\n", cli->ptr_this_struct_9E4);

	struct dump_skip_t skip_info[] = {
		{ (offsetof(struct cli_data_t, ptr_struct_import_data_000) + 2), 6 },
		{ (offsetof(struct cli_data_t, ptr_struct_aa_002) + 2), 6 },
		{ (offsetof(struct cli_data_t, file_import_dat_704) + 2), 6 },
		{ (offsetof(struct cli_data_t, file_prefs_prf_706) + 2), 6 },
		{ (offsetof(struct cli_data_t, ptr_offset_9B2) + 2), 6 },
		{ (offsetof(struct cli_data_t, ptr_offset_9EE) + 2), 6 },
		{ (offsetof(struct cli_data_t, ptr_offset_9F0) + 2), 6 },
		{ (offsetof(struct cli_data_t, ptr_this_struct_9E4) + 2), 6 },
		{ (offsetof(struct cli_data_t, ptr_fx_9AE) + 4), 4 },
	};

	dump_struct_data(0x160E, cli, sizeof(struct cli_data_t), skip_info, sizeof(skip_info)/sizeof(struct dump_skip_t));
}

void dump_struct_aa_t(const struct aa_t* aa) {
	printf("sizeof(struct aa_t)=%zu\n", sizeof(struct aa_t));
	dump_struct_data(0x224A, aa, sizeof(struct aa_t), NULL, 0);
}

void dump_struct_import_data_t(const struct import_data_t* import_data) {
	printf("sizeof(struct import_data_t)=%zu\n", sizeof(struct import_data_t));
	dump_struct_data(0x222A, import_data, sizeof(struct import_data_t), NULL, 0);
}

//int my_fx_parse_data_for_struct(struct cli_data_t** __cli_struct, uint16_t* arg_2_var_6, uint16_t arg_3_var_c, uint16_t arg_4_var_a) {
int my_fx_parse_data_for_struct(struct cli_data_t** __cli_struct, struct main_struct_112_t* ptr_to_main_struct_112, int (*the_fx)(void*, void*)) {
	// from main:
	//		mystruct = struct*
	//		ptr_to_main_struct_112 = ptr to main_struct_112
	//		var_c = 0xDB2 (constant value)
	//		var_a = seg001
	uint8_t file_path[256];
	uint8_t var_304[256];
	uint8_t var_204[256];
	uint8_t base_path[256];
	FILE* file_ptr;
	uint16_t error = 0;

	struct cli_data_t* cli_ptr = NULL;  // not in asm, just make things easier

	error = alloc_cli_struct(__cli_struct);
	if (error != 0) {
		printf("alloc_cli_struct failed; error=%d\n", error);
		goto exit;
	}

	cli_ptr = *__cli_struct; // not in asm, just make things easier

	error = my_fx_srand_init(&ptr_to_main_struct_112);
	if (error) {
		printf("my_fx_srand_init failed; error=%d\n", error);
		goto exit;
	}

	// Code seems to do the following at this point (partly based on certain segment values):
	// * base_path & var_204 have some proprietary form of the base path (perhaps working dir)
	// * var_304 appears unchanged
	// * app_name contains "CLI.EXE"
	// * (copy of) struct word is 1 (see when/why this changed @ seg005:0061)
	// {based on further inspection, it appears that multiple path locations are set
	//  and attempted to open the files, possibly trying various path formats}
	//error = some_fx(base_path, var_204, var_304, ((uint8_t*)cli_ptr->application_name_004), (*__cli_struct)->var_offset_9E2);
	strcpy(var_204, base_path_str);
	strcpy(base_path, base_path_str);
	strcpy(cli_ptr->application_name_004, "CLI.EXE");

	// Continue normally again (seg005:0074)
	// Prep for import.dat
	strncpy(file_path, base_path, 256);
	strncat(file_path, SETUP_DIR, 256);
	strncat(file_path, IMPORT_FILE, 256);
	file_ptr = fopen(file_path, "rb");
	if (file_ptr == NULL) {
		printf("Error opening file: %s\n", file_path);
		printf("!!!If this message is displayed, further conversion of assembly code is required!!!\n");
		error = -10;
		goto exit;
	}

	strncpy(cli_ptr->path_to_base_dir_104, base_path, 256); // base_path contains the proprietary form in the asm
	if (file_ptr == NULL) {
		printf("File does not appear to be open (loc 1)\n");
		error = 1;
		goto exit;
	} else {
		fclose(file_ptr);
		file_ptr = NULL; // Not in asm
	}

	// asm tests for error here, and exits if set
	// Set setup dir
	strncpy(cli_ptr->path_to_setup_dir_204, cli_ptr->path_to_base_dir_104, 256);
	strncat(cli_ptr->path_to_setup_dir_204, SETUP_DIR, 256);

	// Set import.dat path
	strncpy(cli_ptr->path_to_import_dat_304, cli_ptr->path_to_setup_dir_204, 256);
	strncat(cli_ptr->path_to_import_dat_304, IMPORT_FILE, 256);

	// Set prefs.prf path
	strncpy(cli_ptr->path_to_prefs_prf_404, cli_ptr->path_to_setup_dir_204, 256);
	strncat(cli_ptr->path_to_prefs_prf_404, PREFS_FILE, 256);

	// Set sysinfo.dos path
	strncpy(cli_ptr->path_to_sysinfo_dos_504, cli_ptr->path_to_setup_dir_204, 256);
	strncat(cli_ptr->path_to_sysinfo_dos_504, SYSINFO_FILE, 256);

	// Set errors.dat path
	strncpy(cli_ptr->path_to_error_dat_604, cli_ptr->path_to_setup_dir_204, 256);
	strncat(cli_ptr->path_to_error_dat_604, ERROR_FILE, 256);

	// Set the function pointer
	cli_ptr->ptr_fx_9AE = the_fx;
	//cli_ptr->var_offset_9B0 = arg_4_var_a;
	//cli_ptr->var_offset_9AE = arg_3_var_c;
	cli_ptr->ptr_offset_9B2 = ptr_to_main_struct_112;

	error = read_import_dat(cli_ptr, &(cli_ptr->ptr_struct_aa_002), cli_ptr);

#if 0
	printf("cli_ptr:\n");
	//print_hex(cli_ptr_a, sizeof(struct cli_data_t));
	dump_struct_cli_t(cli_ptr);
	dump_struct_import_data_t(cli_ptr->ptr_struct_import_data_000);
	dump_struct_aa_t(cli_ptr->ptr_struct_aa_002);

	//0x1A
	printf("Buffer cli_ptr->ptr_offset_9EE (@2392):\n");
	print_hex(cli_ptr->ptr_offset_9EE, 0x1A);
	printf("Buffer cli_ptr->ptr_offset_9F0 (@23B0):\n");
	print_hex(cli_ptr->ptr_offset_9F0, 0x1A);
#endif

exit:
	if (file_ptr) {
		printf("File is still open, closing\n");
		fclose(file_ptr);
		file_ptr = NULL;
	}
	return error;
}

int process_prefs_prf(uint16_t* arg_0, uint16_t* caller_var_4, struct cli_data_t* cli_data) {
	int error = 0;

	if (cli_data == NULL) {
		// seg005:2646
		error = ERR_0x32_GLBL_PTR_NULL;
		goto exit;
	}

	if (cli_data->ptr_this_struct_9E4 != cli_data) {
		// seg005:2651
		error = ERR_0x38_BAD_PARAM;
		goto exit;
	}

	error = read_prefs_prf(cli_data);
	// seg005:266A

	if (!error) {
		if (arg_0) {
			*arg_0 = cli_data->prefs_data_offs_02_820;
		}

		if (caller_var_4) {
			*caller_var_4 = cli_data->prefs_data_offs_06_824;
		}
	} else if (error == ERR_0x1D_PREFS_CREATE_UPDATE) {
		// seg005:26A0
		if (arg_0) {
			*arg_0 = 0;
		}
		if (caller_var_4) {
			*caller_var_4 = 0;
		}
		error = 0;
	}

exit:
	return error;
}

int my_fx_prefs_prf(int* argc, char** argv, uint16_t* error, struct cli_data_t* cli_data, struct main_struct_112_t* ptr_to_main_struct_112) {
	int ret_val = 0; // Not in asm
	char buffer[256];
	char path[256];

	int var_4;

	memset(path, 0, 256); // ASM seems to copy zeroed block to path
	memset(buffer, 0, 256); // ASM seems to copy zeroed block to buffer

	var_4 = 0;
	*error = 0;

	if (ptr_to_main_struct_112->var_byte_offset_5 != 0) {
		printf("WHD: ptr_to_main_struct_112->var_byte_offset_5 != 0\n");
		// Some sort of gets to ptr_to_main_struct_112->buffer_offset_6
		// -> if failure (ie gets returns null/0), return with zero
		// Replace with fgets to avoid warning; adjust code accordingly
		// (http://stackoverflow.com/questions/34031514/implicit-declaration-of-gets)
		if (fgets(ptr_to_main_struct_112->buffer_offset_6, sizeof(ptr_to_main_struct_112->buffer_offset_6), stdin) == 0) {
			ret_val = 0;
			goto exit;
		} else {
			// Strip newline
			ptr_to_main_struct_112->buffer_offset_6[strcspn(ptr_to_main_struct_112->buffer_offset_6, "\n")] = '\0';
		}

		// seg001:0062
		printf("my_fx_prefs_prf: processing of input not yet implemented\n");
		// Follow error path for now
		ret_val = 1;
		goto exit;
	} else if (ptr_to_main_struct_112->var_byte_offset_2 != 0) { // Byte compare; might be an else if to [5]??
		printf("WHD: ptr_to_main_struct_112->var_byte_offset_2 != 0\n");
		// seg001:00B1
		ret_val = 0;
		goto exit;
	}

	// seg001:00BC
	if (*argc >= 3) {
		// seg001:00C7
		printf("my_fx_prefs_prf: *argc >= 3 not yet implemented\n");
		ret_val = 0;
		*error = -21;
		goto exit;
	}

	if (*argc == 1) {
		// seg001:0155
		printf("WHD: *argc == 1\n");
		strncpy(buffer, STR_516h, 256);
		printf(FMT_STR_2C5h, buffer, STR_2D2h, STR_2DBh);
		ret_val = 0;
		goto exit;
	} else { // This should mean (*argc > 1)
		// seg001:017C
		if (*error != 0) {
			ret_val = ERR_0x09_DISK_FULL;
			goto exit;
		}

		if (ptr_to_main_struct_112->var_byte_offset_2 == 0) {
			// seg001:0186
			*error = process_prefs_prf(NULL, &var_4, cli_data);
			// seg001:01A9
			if (*error) {
				ret_val = ERR_0x09_DISK_FULL;
				goto exit;
			}

			// seg001:01B4
			if (var_4 == 0) {
				ret_val = ERR_0x02_BAD_LOCK_FILE_HEADER;
				goto exit;
			}

			// seg001:01C1
			ptr_to_main_struct_112->var_byte_offset_2 = 1;

		} else {
			printf("WHD: ptr_to_main_struct_112->var_byte_offset_2 != 0 not yet implemented\n");
			ret_val = 0;
			*error = -22;
			goto exit;
		}
	}

	goto exit;

exit:
	return ret_val;
}

int cli_main(int argc, char** argv) {
	uint8_t buffer[256];
	uint8_t var_13a[40];
	struct main_struct_112_t struct_112; // 1780:FEC4 ?
//	uint8_t arr_112[131]; // 1780:FEC4 ?
	//uint16_t var_112;
	//uint8_t var_110 = 0;
	//uint16_t var_10f = 0; // should include byte 10e
	//uint8_t var_10d = 0;
	//uint8_t var_10c[256]; // ?? not in IDA

	// This is most likely a function pointer
	// This function has been implemented as some_fx() in this file
	//uint16_t var_c = 0x0DB2; // Constant
	//uint16_t var_a = 0x0673; // seg seg001; function pointer?

	uint16_t var_8 = 1;
//	uint8_t* ptr_to_arr_112;
    struct main_struct_112_t* ptr_to_struct_112;
	struct cli_data_t* cli_data = NULL;	// 1780:160E
	uint16_t error = 0;

	struct_112.var_byte_offset_2 = 0;
	struct_112.var_word_offset_3 = 0;
	struct_112.var_byte_offset_5 = 0;
	ptr_to_struct_112 = &struct_112;

	error = my_fx_parse_data_for_struct(&cli_data, ptr_to_struct_112, &some_fx);
	if (error != 0) {
		printf(STR_31Fh, error);
		goto exit;
	}
	// seg001:0E61
	if (isatty(STDIN_FILENO) == 0) {
		// seg001:0E61
		// TODO: figure out what true test is
		printf("WHD: STDIN is not a character device\n");
		strncpy(buffer, STR_516h, 256);
		printf(FMT_STR_2C5h, buffer, STR_2D2h, STR_2DBh);
		// It appears that true output is: printf("%s of %s %s\n", "11:09:29", "Apr 20 1994", "Vendor Version 1.4.1");
		// After this the program seems to enter some loop later on (after changing test value in debug)
		// (Presumably waiting for some sort of file input??)
	}

	//seg001:0EA0
	if (argc > 1) {
		if ((argv[1][0] == 'u') || (argv[1][0] == 'U')) {
			//seg001:0EBC
			printf("TODO: Option 'U' not yet implemented\n");
			error = -1;
			var_8 = 0; // var_8 set by some function call here
		}
	}

	while(var_8) {
		// seg001:0EDA
		switch (var_8) {
			case 1:
				// seg001:0EEC
				var_8 = my_fx_prefs_prf(&argc, argv, &error, cli_data, ptr_to_struct_112);
				break;

			default:
				printf("cli_main() switch default case not yet implemented\n");
				error = -9;
				goto exit;
		}

		var_8 = 0; // WHD just to end loop for now
	}

	// WHD debug:
//	dump_struct_cli_t(cli_data);

	// seg001:0FBA
	printf("WHD: var_8 == 0\n");
	if (free_cli_struct(&cli_data)) {
		printf(STR_346h);
	}
	goto exit;

exit:
	if (cli_data) {
		free_cli_struct(&cli_data);
	}
	return error;
}

int main(int argc, char** argv) {
	int r = 0;
	char file_path[256]; // Should be long enough

	int cli_argc = 0;
	char** cli_argv = NULL;

	r = parse_args(argc, argv);
	if (r) {
		// parse_args should have printed an error message
		return -1;
	}

	printf("SEEK_SET: %d\n", SEEK_SET);
	printf("SEEK_CUR: %d\n", SEEK_CUR);
	printf("SEEK_END: %d\n", SEEK_END);
/*
	// Attempt to open all the required files
	// vendinit.exe
	set_file_path(file_path, VENDINIT_FILE);
	vendinit_file = fopen(file_path, "rb");
	if (vendinit_file == NULL) {
		printf("Unable to open file %s\n", file_path);
		r = -2;
		goto exit;
	}

	// error.dat
	set_file_path(file_path, IMPORT_FILE);
	error_file = fopen(file_path, "rb");
	if (error_file == NULL) {
		printf("Unable to open file %s\n", file_path);
		r = -4;
		goto exit;
	}

	// import.dat
	set_file_path(file_path, IMPORT_FILE);
	import_file = fopen(file_path, "rb");
	if (import_file == NULL) {
		printf("Unable to open file %s\n", file_path);
		r = -4;
		goto exit;
	}

	// prefs.prf
	set_file_path(file_path, PREFS_FILE);
	prefs_file = fopen(file_path, "rb");
	if (prefs_file == NULL) {
		printf("Unable to open file %s\n", file_path);
		r = -4;
		goto exit;
	}
*/

	//r = my_fx_test_group_of_seven(ptr_gos);
	printf("base_path_str=%s\n", base_path_str);

	char cli_cmd_str[512];

	memset(cli_cmd_str, 0, 512);
	if (cli_cmd_line_args != NULL) {
		strcat(cli_cmd_str, "CLI.EXE ");
		strcat(cli_cmd_str, cli_cmd_line_args);
	} else {
		strcat(cli_cmd_str, "CLI.EXE");
	}

	printf("cli_cmd_str: %s\n", cli_cmd_str);

	r = build_arg_list(cli_cmd_str, &cli_argc, &cli_argv);
	if (r) {
		printf("Error parsing CLI command line\n");
		r = -10;
		goto exit;
	}

	printf("CLI.EXE argument count: %d\n", cli_argc);
	for (int i = 0; i < cli_argc; ++i) {
		printf("Arg %d: %s\n", i, cli_argv[i]);
	}
	cli_main(cli_argc, cli_argv);


	// WHD: cryto test, remove
//	uint16_t init_vect[] = {0xA9F4, 0xA9F4, 0xA9F4, 0xA9F4};
//	uint8_t my_buf[0x90];
//	uint8_t* my_ptr = NULL;
//
//	memset(my_buf, 0, 0x90);
//
//	my_ptr = my_buf + 0x08;
//	strcpy(my_ptr, "string 1");
//	my_ptr = my_buf + 0x32;
//	strcpy(my_ptr, "another one, string 2");
//	my_ptr = my_buf + 0x54;
//	strcpy(my_ptr, "a third string");
//	my_ptr = my_buf + 0x73;
//	strcpy(my_ptr, "this is sommer nog a string");

//	printf("my_buf, pre crypt:\n");
//	print_hex(my_buf, 0x90);
//
//	cli_crypto_decrypt(my_buf, 0x90, 0, init_vect);
//
//	printf("my_buf, post crypt:\n");
//	print_hex(my_buf, 0x90);
//
//	cli_crypto_decrypt(my_buf, 0x90, 0, init_vect);
//
//	printf("my_buf, post post crypt:\n");
//	print_hex(my_buf, 0x90);

exit:
	// perform cleanup
/*
	if (vendinit_file) {
		fclose(vendinit_file);
		vendinit_file = NULL;
	}
	if (error_file) {
		fclose(error_file);
		error_file = NULL;
	}
	if (import_file) {
		fclose(import_file);
		import_file = NULL;
	}
	if (prefs_file) {
		fclose(prefs_file);
		prefs_file = NULL;
	}
*/
	if (cli_argv) {
		free_arg_list(cli_argv);
		cli_argv = NULL;
	}
	return r;
}
