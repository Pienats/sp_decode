#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include "cli.h"
#include "cli_crypto.h"
#include "cli_prefs_prf.h"
#include "cli_file_util.h"
#include "cli_errors.h"

#define MODE_RB "rb"
#define MODE_RB_PLUS "rb+"

static int close_prefs_prf_if_open(struct cli_data_t* cli_data) {
//	int var_1 = 0;

	if (cli_data->file_prefs_prf_706) {
		fclose(cli_data->file_prefs_prf_706);
		cli_data->file_prefs_prf_706 = NULL;
	}

	// seg010:02D3
	// Not sure why var_1 tested, as it's not clear where it would change

	return 0;
}

static int open_and_reset_prefs_prf(int action, struct cli_data_t* cli_data) {
	int error = 0;
	char* mode = NULL; // Not directly in asm

	if ((action == 1) || (action == 4)) {
		error = close_prefs_prf_if_open(cli_data);

		// Remove the PREFS.PRF file
		unlink(cli_data->path_to_prefs_prf_404);

		// Open  the PREFS.PRF file for writing
		// seg010:01F3
		cli_data->file_prefs_prf_706 = fopen(cli_data->path_to_prefs_prf_404, "wb");
		close_prefs_prf_if_open(cli_data);
	}

	if ((action != 4) && (!error)) {
		close_prefs_prf_if_open(cli_data);

		if (action == ACTION_OPEN_FOR_READ) {
			mode = MODE_RB;
		} else {
			if (cli_struct_valid(cli_data)) {
				mode = MODE_RB_PLUS;
			} else {
				printf("open_prefs_prf: cli structure not valid, call MS-DOS interrupt\n");
				error = -30;
				goto exit;
			}
		}

		cli_data->file_prefs_prf_706 = fopen(cli_data->path_to_prefs_prf_404, mode);

		if (!cli_data->file_prefs_prf_706) {
			error = ERR_0x1D_PREFS_CREATE_UPDATE;
			goto exit;
		}

		if (fseek(cli_data->file_prefs_prf_706, 0, SEEK_SET) != 0) {
			error = ERR_0x1E_PREFS_READ;
			goto exit;
		}
	}

exit:
	return error;
}

int read_prefs_prf(struct cli_data_t* cli_data) {
	uint8_t* ptr_var;
	uint16_t chksum_calc = 0;
	uint16_t chksum_read = 0;
	uint8_t* prefs_data = NULL;
	uint16_t var_6 = 0;
	uint16_t prefs_version;
	int error = 0;

	if (cli_data == NULL) {
		error = ERR_0x36_NULL_PTR;
		goto exit;
	}

	// seg010:0823
	error = open_and_reset_prefs_prf(ACTION_OPEN_FOR_READ, cli_data);
	if (error) {
		goto exit;
	}

	// seg010:083C
	if (fread_swap_bytes(&prefs_version, 1, 2, cli_data->file_prefs_prf_706, 1, 0) != 2) {
		error = ERR_0x1E_PREFS_READ;
	}

	printf("Prefs.prf version: %d\n", prefs_version);

	if (!error) {
		if ((prefs_version != 2) && (prefs_version != 3) && (prefs_version != 4)) {
			error = ERR_0x3A_VERSION;
		}
	}

	if (!error) {
		// seg010:088E
		prefs_data = calloc(1, 0x90); // 144
		if (fread_swap_bytes(prefs_data, 1, 0x90, cli_data->file_prefs_prf_706, 1, 0) != 0x90) {
			error = ERR_0x1E_PREFS_READ;
		}
	}

	if (!error) {
		uint16_t init_vect[] = {0xA9F4, 0xA9F4, 0xA9F4, 0xA9F4};
		// seg010:08EE
		cli_crypto_decrypt(prefs_data, 0x90, 0, init_vect);
		print_hex(prefs_data, 0x90);

		// Validate the checksum agains the stored value
		chksum_read = *(uint16_t*)(prefs_data);
		*(uint16_t*)(prefs_data) = 0;

		ptr_var = prefs_data;
		while (ptr_var < (prefs_data + 0x90)) {
			chksum_calc ^= *ptr_var;
			chksum_calc += *ptr_var;
			ptr_var++;
		}

		printf("chksum_read: 0x%04X\n", chksum_read);
		printf("chksum_calc: 0x%04X\n", chksum_calc);
		if (chksum_read != chksum_calc) {
			printf("chksum_read != chksum_calc\n");
			error = ERR_0x1E_PREFS_READ;
		}
	}

	if (!error) {
		// seg010:0943
		cli_data->prefs_data_offs_02_820 = *(uint16_t*)(prefs_data + 2);
		var_6 = *(uint16_t*)(prefs_data + 4);
		cli_data->prefs_data_offs_06_824 = *(uint16_t*)(prefs_data + 6);
		cli_data->prefs_data_offs_08_826 = *(uint16_t*)(prefs_data + 8);
		cli_data->prefs_data_offs_0A_82A = *(uint16_t*)(prefs_data + 10);
		cli_data->prefs_data_offs_0C_828 = *(uint16_t*)(prefs_data + 12);
		cli_data->prefs_data_offs_0E_82C = *(uint16_t*)(prefs_data + 14);
		cli_data->prefs_data_offs_10_82E = *(uint32_t*)(prefs_data + 16);
		cli_data->prefs_data_offs_14_832 = *(uint16_t*)(prefs_data + 20);

		// seg010:09C2
		strncpy(cli_data->string_title_836, (prefs_data + 24), 60);
		strncpy(cli_data->string_producer_872, (prefs_data + 84), 60);
	}

	if (prefs_data) {
		free(prefs_data);
		prefs_data = NULL;
	}

	// seg010:09FF
	cli_data->prefs_data_offs_04_9F6 = var_6;

	if (var_6 > 0) {
		cli_data->var_offset_822 = (var_6 - 1);
	} else {
		cli_data->var_offset_822 = 0;
	}

exit:
	// Perform cleanup
	close_prefs_prf_if_open(cli_data);
	if (prefs_data) {
		printf("prefs_data not yet freed, doing so now");
		free(prefs_data);
		prefs_data = NULL;
	}
	return error;
}

