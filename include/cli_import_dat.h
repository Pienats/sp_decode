#ifndef __CLI_IMPORT_DAT_H__
#define __CLI_IMPORT_DAT_H__

/**
 * @brief Read the IMPORT.DAT file
 * @param cli_ptr_a CLI data structure
 * @param struct_ptr_offs_002 Structure stored at offset 002
 * @param cli_ptr_b CLI data structure
 * @return Zero on success; Greater than zero for CLI error; Less than zero for unhandled cases
 */
int read_import_dat(struct cli_data_t* cli_ptr_a, struct aa_t** struct_ptr_offs_002, struct cli_data_t* cli_ptr_b);

#endif // __CLI_IMPORT_DAT_H__
