#ifndef __CLI_H__
#define __CLI_H__

#define BASE_PATH "./"
#define SETUP_DIR "SETUP/"

#define VENDINIT_FILE "VENDINIT.EXE"
#define IMPORT_FILE "IMPORT.DAT"		// in /SETUP
#define PREFS_FILE "PREFS.PRF"			// in /SETUP
#define SYSINFO_FILE "SYSINFO.DOS"		// in /SETUP
#define ERROR_FILE "ERRORS.DAT" 		// in /SETUP

#define VAR_304_STR = ""

// "Error" strings
#define FMT_STR_2C5h "%s of %s %s\n"

#define STR_2D2h "11:09:29"
#define STR_2DBh "Apr 20 1994"
#define STR_31Fh "Critical Error on Initialization - %d\n"
#define STR_346h "Critical Error on Shut Down\n"
#define STR_516h "Vendor Version 1.4.1"


/**
 * This seems to be the structure at 1780:222A
 * (address might vary between runs, but so far
 * it has been pretty consistant)
 */
struct import_data_t {
	// offset 0x000 (0x222A)
	uint32_t var_long_offset_000;
	// offset 0x004 (0x222E)
	uint32_t var_long_offset_004;
	// offset 0x008 (0x2232)
	uint32_t var_long_offset_008;
	// offset 0x00C (0x2236)
	uint32_t var_long_count_A_00C;		// Some sort of count/unit size (used for alloc @seg009:08D4)
	// offset 0x010 (0x223A)
	uint32_t var_long_count_B_010;		// Some sort of count/unit size (used for alloc @seg009:089A)
	// offset 0x014 (0x223E)
	uint32_t var_long_offset_014;		// Absolute size of block of var_long_count_B_010 units
	// offset 0x018 (0x2242)
	uint32_t block_count_offset_018;	// Some sort of count
} __attribute__((packed));

/**
 * This seems to be the structure at 1780:224A
 * (address might vary between runs, but so far
 * it has been pretty consistant)
 */
struct aa_t {
	// offset 0x000 (0x224A)
	uint32_t var_long_offset_000;
	// offset 0x004 (0x224E)
	uint32_t var_long_offset_004;
	// offset 0x008 (0x2252)
	uint32_t var_long_offset_008;
	// offset 0x00C (0x2256)
	uint32_t var_long_offset_00C;
	// offset 0x010 (0x225A)
	uint32_t var_long_offset_010;
	// offset 0x014 (0x225E)
	uint32_t var_long_offset_014;
	// offset 0x018 (0x2262)
	uint32_t var_long_offset_018;
	// offset 0x01C (0x2266)
	uint32_t var_long_offset_01C;
	// offset 0x020 (0x226A)
	uint8_t array_offset_020[32]; // offset 030 (228A) seems to have a path to one of the vendxx files
	// offset 0x040 (0x22AA)
	uint8_t array_offset_040[256];
//	// offset 0x000 (0x224A)
//	uint8_t array_offset_000[20];
//	// offset 0x014 (0x223E)
//	uint32_t var_long_offset_014;
	// offset 0x0140 (0x2242)
	uint32_t var_long_offset_140;	// Some sort of count
} __attribute__((packed));

struct main_struct_112_t {
    uint16_t srand_val_offset_0;
    uint8_t var_byte_offset_2;
    uint16_t var_word_offset_3;
    uint8_t var_byte_offset_5;
	uint8_t buffer_offset_6[256];
} __attribute__((packed));

/**
 * This seems to be the structure at 1780:160E
 * (address might vary between runs, but so far
 * it has been pretty consistant)
 */
struct cli_data_t {
	// import.dat related? (look at seg005:0336)
	// read_import_dat:arg0
	//uint8_t* ptr_offset_000; // [word ptr ?] Group of seven (might be another struct that contains this)
	struct import_data_t* ptr_struct_import_data_000;	// points to 0x222A

	// offset 0x002
	// read_import_dat:arg2
	struct aa_t* ptr_struct_aa_002; // xx // points to 224A

	// guesing as to what this is
	// offset 0x004 (0x1612)
	uint8_t application_name_004[256]; //CLI.EXE

	// offset 0x104 (0x1712)
	uint8_t path_to_base_dir_104[256];
	// offset 0x204 (0x1812)
	uint8_t path_to_setup_dir_204[256];
	// offset 0x304 (0x1912)
	uint8_t path_to_import_dat_304[256];
	// offset 0x404 (0x1A12)
	uint8_t path_to_prefs_prf_404[256];
	// offset 0x504 (0x1B12)
	uint8_t path_to_sysinfo_dos_504[256];
	// offset 0x604 (0x1C12)
	uint8_t path_to_error_dat_604[256];
	// offset 0x704 (0x1D12)
	FILE* file_import_dat_704; // file pointer to import.dat
	// offset 0x706 (0x1D14)
	FILE* file_prefs_prf_706; // file pointer to prefs.prf
	// offset 0x708 (0x1D16)
	uint16_t var_offset_708;
	// offset 0x70A (0x1D18)
	uint16_t var_offset_70A;
	// offset 0x70C (0x1D1A)
	uint16_t var_offset_70C;
	// offset 0x70E (0x1D1C)
	uint16_t var_offset_70E;
	// offset 0x710 (0x1D1E)
	uint8_t array_offset_710[256];
	// offset 0x810 (0x1E1E)
	uint16_t var_offset_810;
	// offset 0x812 (0x1E20)
	uint16_t var_offset_812;
	// offset 0x814 (0x1E22)
	uint16_t var_offset_814;
	// offset 0x816 (0x1E24)
	uint16_t var_offset_816;
	// offset 0x818 (0x1E26)
	uint16_t var_offset_818;
	// offset 0x81A (0x1E28)
	uint16_t var_offset_81A;
	// offset 0x81C (0x1E2A)
	uint16_t var_offset_81C;
	// offset 0x81E (0x1E2C)
	uint16_t var_offset_81E;
	// offset 0x820 (0x1E2E)
	uint16_t prefs_data_offs_02_820;
	// offset 0x822 (0x1E30)
	uint16_t var_offset_822;
	// offset 0x824 (0x1E32)
	uint16_t prefs_data_offs_06_824;
	// offset 0x826 (0x1E34)
	uint16_t prefs_data_offs_08_826;
	// offset 0x828 (0x1E36)
	uint16_t prefs_data_offs_0C_828;
	// offset 0x82A (0x1E38)
	uint16_t prefs_data_offs_0A_82A;
	// offset 0x82C (0x1E3A)
	uint16_t prefs_data_offs_0E_82C;
	// offset 0x82E (0x1E3C)
	uint32_t prefs_data_offs_10_82E;	//	uint16_t var_offset_82E;
										//	// offset 0x830 (0x1E3E)
										//	uint16_t var_offset_830;
	// offset 0x832 (0x1E40)
	uint32_t prefs_data_offs_14_832;	// uint16_t var_offset_832
										//	// offset 0x834 (0x1E42)
										//	uint16_t var_offset_834;
	// offset 0x836 (0x1E44)
	uint8_t string_title_836[60];
	// offset 0x872 (0x1E80)
	uint8_t string_producer_872[60];
	// offset 0x8AE (0x1EBC)
	uint8_t array_offset_8AE[256];

	// offset 0x9AE (0x1FBC) // set @ seg005:0313 from arg_4
	// function pointer				// offset 0x9AE (0x1FBC) // set @ seg005:0313 from arg_4
	int (*ptr_fx_9AE)(void*, void*);// uint16_t var_offset_9AE; // var_c;
									// offset 0x9B0 (0x1FBE) // set @ seg005:030F from arg_6
									// uint16_t var_offset_9B0; // var_a -> might be a code segment (seg001)


	// offset 0x9B2 (0x1FC0) // set @ seg005:031C from arg_2 (appears to have something to do with srand(NULL)
	struct main_struct_112_t* ptr_offset_9B2;
	// offset 0x9B4 (0x1FC2)
	uint16_t var_offset_9B4;
	// offset 0x9B6 (0x1FC4)
	uint16_t var_offset_9B6;
	// offset 0x9B8 (0x1FC6)
	uint16_t var_offset_9B8;
	// offset 0x9BA (0x1FC8)
	uint16_t var_offset_9BA;
	// offset 0x9BC (0x1FCA)
	uint16_t var_offset_9BC;
	// offset 0x9BE (0x1FCC)
	uint16_t var_offset_9BE;
	// offset 0x9C0 (0x1FCE)
	uint32_t import_dat_version_9C0;		// offset 0x9C0 (0x1FCE)
											//uint16_t var_offset_9C0;
											// offset 0x9C2 (0x1FD0)
											//uint16_t var_offset_9C2;
	// offset 0x9C4 (0x1FD2)
	uint32_t var_long_offset_9C4;			//uint16_t var_offset_9C4;
											// offset 0x9C6 (0x1FD4)
											//uint16_t var_offset_9C6;
	// offset 0x9C8 (0x1FD6)
	uint16_t var_offset_9C8;
	// offset 0x9CA (0x1FD8)
	uint32_t var_long_offset_9CA;			// uint16_t var_offset_9CA;
											// offset 0x9CC (0x1FDA)
											// uint16_t var_offset_9CC;
	// offset 0x9CE (0x1FDC)
	uint32_t var_long_offset_9CE;			// uint16_t var_offset_9CE;
											// offset 0x9D0 (0x1FDE)
											// uint16_t var_offset_9D0;
	// offset 0x9D2 (0x1FE0)
	uint16_t var_offset_9D2;
	// offset 0x9D4 (0x1FE2)
	uint16_t var_offset_9D4;
	// offset 0x9D6 (0x1FE4)
	uint16_t var_offset_9D6;
	// offset 0x9D8 (0x1FE6)
	uint16_t var_offset_9D8;
	// offset 0x9DA (0x1FE8)
	uint16_t var_offset_9DA;
	// offset 0x9DC (0x1FEA)
	uint16_t var_offset_9DC;
	// offset 0x9DE (0x1FEC)
	uint16_t var_offset_9DE;
	// offset 0x9E0 (0x1FEE)
	uint16_t var_offset_9E0;
	// offset 0x9E2 (0x1FF0)
	uint16_t var_offset_9E2;
	// offset 0x9E4 (0x1FF2) // set @ seg012:0048 from ptr
	struct cli_data_t* ptr_this_struct_9E4; // ?
	// offset 0x9E6 (0x1FF4)
	uint32_t var_long_offset_9E6;			// uint16_t var_offset_9E6;
											// offset 0x9E8 (0x1FF6)
											// uint16_t var_offset_9E8;
	// offset 0x9EA (0x1FF8)
	uint32_t var_long_file_offset_9EA;		// uint16_t var_offset_9EA;
											// offset 0x9EC (0x1FFA)
											// uint16_t var_offset_9EC;
	// offset 0x9EE (0x1FFC)
	uint8_t* ptr_offset_9EE;
	// offset 0x9F0 (0x1FFE)
	uint8_t* ptr_offset_9F0;
	// offset 0x9F2 (0x2000)
	uint32_t var_long_offset_9F2;			// uint16_t var_offset_9F2;
											// offset 0x9F4 (0x2002)
											// uint16_t var_offset_9F4;
	// offset 0x9F6 (0x2004)
	uint16_t prefs_data_offs_04_9F6;
	// offset 0x9F8 (0x2006)
	uint16_t var_offset_9F8;
	// offset 0x9FA (0x2008)
	uint16_t var_offset_9FA;
	// offset 0x9FC (0x200A)
	uint16_t var_offset_9FC;
	// offset 0x9FE (0x200C)
	uint16_t var_offset_9FE;
	// offset 0xA00 (0x200E)
	uint32_t var_long_offset_A00;			// uint16_t var_offset_A00;
											// offset 0xA02 (0x2010)
											// uint16_t var_offset_A02;
	// offset 0xA04 (0x2012)
	uint32_t var_long_offset_A04;			// uint16_t var_offset_A04;
											// offset 0xA06 (0x2014)
											// uint16_t var_offset_A06;
	// offset 0xA08 (0x2016)
	uint32_t var_long_offset_A08;			// uint16_t var_offset_A08;
											// offset 0xA0A (0x2018)
											// uint16_t var_offset_A0A;
	// offset 0xA0C (0x201A)
	uint32_t var_long_offset_A0C;			// uint16_t var_offset_A0C;
											// offset 0xA0E (0x201C)
											// uint16_t var_offset_A0E;
	// offset 0xA10 (0x201E)
	uint32_t var_long_offset_A10;			// uint16_t var_offset_A10;
											// offset 0xA12 (0x2020)
											// uint16_t var_offset_A12;

	// size = 0xA14
} __attribute__((packed));

/**
 * @brief Provide a printable character
 * @param in_char Character to be converted
 * @return in_char if in_char is printable, '.' otherwise
 */
char to_printable(char in_char);

/**
 * @brief Print a buffer in hex output
 * @param data Data buffer to print
 * @param len Length of buffer
 */
void print_hex(const void* data, size_t len);

/**
 * @brief Basic validation on the CLI data structure
 * @param cli_data CLI data structure to validate
 * @return One if the CLI data structure is valid, false otherwise
 */
int cli_struct_valid(struct cli_data_t* cli_data);

#endif

