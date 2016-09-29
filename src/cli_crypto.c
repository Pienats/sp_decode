#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cli.h"
#include "cli_crypto.h"

static uint8_t lookup_table[] = {
	0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C,
	0x40, 0x44, 0x48, 0x4C, 0x50, 0x54, 0x58, 0x5C, 0x60, 0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x7C,
	0x80, 0x84, 0x88, 0x8C, 0x90, 0x94, 0x98, 0x9C, 0xA0, 0xA4, 0xA8, 0xAC, 0xB0, 0xB4, 0xB8, 0xBC,
	0xC0, 0xC4, 0xC8, 0xCC, 0xD0, 0xD4, 0xD8, 0xDC, 0xE0, 0xE4, 0xE8, 0xEC, 0xF0, 0xF4, 0xF8, 0xFC,
	0x01, 0x05, 0x09, 0x0D, 0x11, 0x15, 0x19, 0x1D, 0x21, 0x25, 0x29, 0x2D, 0x31, 0x35, 0x39, 0x3D,
	0x41, 0x45, 0x49, 0x4D, 0x51, 0x55, 0x59, 0x5D, 0x61, 0x65, 0x69, 0x6D, 0x71, 0x75, 0x79, 0x7D,
	0x81, 0x85, 0x89, 0x8D, 0x91, 0x95, 0x99, 0x9D, 0xA1, 0xA5, 0xA9, 0xAD, 0xB1, 0xB5, 0xB9, 0xBD,
	0xC1, 0xC5, 0xC9, 0xCD, 0xD1, 0xD5, 0xD9, 0xDD, 0xE1, 0xE5, 0xE9, 0xED, 0xF1, 0xF5, 0xF9, 0xFD,
	0x02, 0x06, 0x0A, 0x0E, 0x12, 0x16, 0x1A, 0x1E, 0x22, 0x26, 0x2A, 0x2E, 0x32, 0x36, 0x3A, 0x3E,
	0x42, 0x46, 0x4A, 0x4E, 0x52, 0x56, 0x5A, 0x5E, 0x62, 0x66, 0x6A, 0x6E, 0x72, 0x76, 0x7A, 0x7E,
	0x82, 0x86, 0x8A, 0x8E, 0x92, 0x96, 0x9A, 0x9E, 0xA2, 0xA6, 0xAA, 0xAE, 0xB2, 0xB6, 0xBA, 0xBE,
	0xC2, 0xC6, 0xCA, 0xCE, 0xD2, 0xD6, 0xDA, 0xDE, 0xE2, 0xE6, 0xEA, 0xEE, 0xF2, 0xF6, 0xFA, 0xFE,
	0x03, 0x07, 0x0B, 0x0F, 0x13, 0x17, 0x1B, 0x1F, 0x23, 0x27, 0x2B, 0x2F, 0x33, 0x37, 0x3B, 0x3F,
	0x43, 0x47, 0x4B, 0x4F, 0x53, 0x57, 0x5B, 0x5F, 0x63, 0x67, 0x6B, 0x6F, 0x73, 0x77, 0x7B, 0x7F,
	0x83, 0x87, 0x8B, 0x8F, 0x93, 0x97, 0x9B, 0x9F, 0xA3, 0xA7, 0xAB, 0xAF, 0xB3, 0xB7, 0xBB, 0xBF,
	0xC3, 0xC7, 0xCB, 0xCF, 0xD3, 0xD7, 0xDB, 0xDF, 0xE3, 0xE7, 0xEB, 0xEF, 0xF3, 0xF7, 0xFB, 0xFF,
};


static void initialize_crypto_vector(uint8_t* initial_vector, uint8_t* data_to_update) {
	// seg008:000B
	uint8_t lookup_idx; // Not in asm
	uint16_t update_idx;
	uint8_t outer_count;
	int offset;
	uint8_t count;
	uint8_t array_C[4];
	uint8_t array_8[4];
	uint8_t array_4[4];

	memset(array_C, 0, 4);

	// seg008:0024
	for (outer_count = 0; outer_count < 8; ++outer_count) {
		for (count = 0; count < 4; ++count) {
			offset = (count & 0xFF);

			array_4[offset] = initial_vector[offset + 4];
			array_8[offset] = initial_vector[offset + 4] ^ array_C[offset];
			array_C[offset] = initial_vector[offset];
		}

		initial_vector[5] = initial_vector[1] ^ initial_vector[0];
		initial_vector[6] = initial_vector[3] ^ initial_vector[2];

		lookup_idx = (initial_vector[5] + ((initial_vector[6] ^ array_8[0]) + 1)) & 0xFF;
		initial_vector[5] = lookup_table[lookup_idx];
		lookup_idx = (initial_vector[6] + (initial_vector[5] ^ array_8[1])) & 0xFF;
		initial_vector[6] = lookup_table[lookup_idx];
		lookup_idx = (initial_vector[0] + (initial_vector[5] ^ array_8[2])) & 0xFF;
		initial_vector[4] = lookup_table[lookup_idx];
		lookup_idx = (initial_vector[3] + ((initial_vector[6] ^ array_8[3]) + 1)) & 0xFF;
		initial_vector[7] = lookup_table[lookup_idx];

		update_idx = (outer_count <<2);

		for (count = 0; count < 4; ++count) {
			offset = (count & 0xFF);
			initial_vector[offset] = array_4[offset];
			data_to_update[offset + update_idx] = initial_vector[offset + 4];
		}
	}
}

static void cli_crypto_deobfuscate(uint8_t* crypto_data, void* processing_vector, size_t crypto_data_len, int unknown) {
	uint8_t lookup_idx; // Not in asm

	// seg008:02F7
	uint16_t loop_count;

	uint8_t* ptr_A;
	uint8_t* ptr_C;

	uint16_t tmp_prefs_6;
	uint16_t tmp_prefs_4;

	int set = 0;
	// seg008:048E
	while ((unknown > 0) || (crypto_data_len > 0)) {
		if (unknown != 0) {
			break; // seg008:0497
		}

		// seg008:032B
		//*(uint16_t*)(prefs_data + (set * 8)) ^= *(uint16_t*)(processing_vector + 24);
		*(uint16_t*)(crypto_data + (set * 8) + 0) ^= *(uint16_t*)(processing_vector + 24);
		*(uint16_t*)(crypto_data + (set * 8) + 2) ^= *(uint16_t*)(processing_vector + 26);
		*(uint16_t*)(crypto_data + (set * 8) + 4) ^= *(uint16_t*)(processing_vector + 28);
		*(uint16_t*)(crypto_data + (set * 8) + 6) ^= *(uint16_t*)(processing_vector + 30);

		*(uint16_t*)(crypto_data + (set * 8) + 4) ^= *(uint16_t*)(crypto_data + (set * 8) + 0);
		*(uint16_t*)(crypto_data + (set * 8) + 6) ^= *(uint16_t*)(crypto_data + (set * 8) + 2);

		ptr_A = (processing_vector + 14);
		ptr_C = (processing_vector + 15);

		for (loop_count = 0; loop_count < 8; ++loop_count) {
			// seg008:0371
			tmp_prefs_6 = *(uint16_t*)(crypto_data + (set * 8) + 6);
			tmp_prefs_4 = *(uint16_t*)(crypto_data + (set * 8) + 4);

			crypto_data[(set * 8) + 5] ^= ((crypto_data[(set * 8) + 4] ^ *ptr_A) & 0xFF);
			crypto_data[(set * 8) + 6] ^= ((crypto_data[(set * 8) + 7] ^ *ptr_C) & 0xFF);
			// seg008:03A4

			lookup_idx = (crypto_data[(set * 8) + 5] + crypto_data[(set * 8) + 6] + 1) & 0xFF;
			crypto_data[(set * 8) + 5] = lookup_table[lookup_idx];

			// seg008:03BD
			lookup_idx = (crypto_data[(set * 8) + 5] + crypto_data[(set * 8) + 6]) & 0xFF;
			crypto_data[(set * 8) + 6] = lookup_table[lookup_idx];
			lookup_idx = (crypto_data[(set * 8) + 4] + crypto_data[(set * 8) + 5]) & 0xFF;
			crypto_data[(set * 8) + 4] = lookup_table[lookup_idx];
			lookup_idx = (crypto_data[(set * 8) + 7] + crypto_data[(set * 8) + 6] + 1) & 0xFF;
			crypto_data[(set * 8) + 7] = lookup_table[lookup_idx];

			// seg008:0401
			*(uint16_t*)(crypto_data + (set * 8) + 4) ^= *(uint16_t*)(crypto_data + (set * 8) + 0);
			*(uint16_t*)(crypto_data + (set * 8) + 6) ^= *(uint16_t*)(crypto_data + (set * 8) + 2);
			*(uint16_t*)(crypto_data + (set * 8) + 2) = tmp_prefs_6;
			*(uint16_t*)(crypto_data + (set * 8) + 0) = tmp_prefs_4;

			ptr_A -= 2;
			ptr_C -= 2;
		}

		// seg008:0433
		*(uint16_t*)(crypto_data + (set * 8) + 4) ^= *(uint16_t*)(crypto_data + (set * 8) + 0);
		*(uint16_t*)(crypto_data + (set * 8) + 6) ^= *(uint16_t*)(crypto_data + (set * 8) + 2);
		*(uint16_t*)(crypto_data + (set * 8) + 0) ^= *(uint16_t*)(crypto_data + (set * 8) + 4);
		*(uint16_t*)(crypto_data + (set * 8) + 2) ^= *(uint16_t*)(crypto_data + (set * 8) + 6);
		*(uint16_t*)(crypto_data + (set * 8) + 0) ^= *(uint16_t*)(processing_vector + 16);
		*(uint16_t*)(crypto_data + (set * 8) + 2) ^= *(uint16_t*)(processing_vector + 18);
		*(uint16_t*)(crypto_data + (set * 8) + 4) ^= *(uint16_t*)(processing_vector + 20);
		*(uint16_t*)(crypto_data + (set * 8) + 6) ^= *(uint16_t*)(processing_vector + 22);

		// seg008:472
		set++;
		crypto_data_len -= 8;
		// seg008:048A -> unknown sbb? how to do this in C //might not need this, as registers should be large enough
	}
}

void cli_crypto_decrypt(void* crypto_data, size_t crypto_data_len, int unknown, uint16_t init_vect[]) {
	// seg008:04EB
	uint8_t crypto_vector[32];
	uint8_t local_init_vect[8];

	memcpy(local_init_vect, init_vect, 8);

	// seg008:0512
	initialize_crypto_vector(local_init_vect, crypto_vector);

	// seg008:0526
	cli_crypto_deobfuscate(crypto_data, crypto_vector, crypto_data_len, unknown);
}


