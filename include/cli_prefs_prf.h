#ifndef __PREFS_PRF_H__
#define __PREFS_PRF_H__

//#define ACTION_RECREATE_READ	1
//#define ACTION_RECREATE_ONLY	4
#define ACTION_OPEN_FOR_READ 2

/**
 * @brief Opens and reads the PREFS.PRF file
 * @param cli_data CLI data structure
 * @return Zero on success; Greater than zero for CLI error; Less than zero for unhandled cases
 */
int read_prefs_prf(struct cli_data_t* cli_data);

#endif // __PREFS_PRF_H__
