#ifndef __ARG_UTIL_H__
#define __ARG_UTIL_H__

/**
 * Build an argument list akin to what would be passed to a normal main() function
 * from a space separated string
 * 
 * @param arg_str Pointer to the string to be parsed
 * @param argc Pointer to where the argument count will be stored
 * @param argv Pointer to where the argument vector will be stored
 * 
 * @return Zero for success, less than zero otherwise
 * 
 * @Note: This function is very simple and not very robust. It only
 * 		  works on simple, space delimited strings. Very little, if
 * 		  any corner case checking is done.
 */
int build_arg_list(const char* arg_str, int* argc, char*** argv);

/**
 * Free the argument vector generted by build_arg_list
 * 
 * @param argv The argument vector to free
 * 
 * @return Zero for success, less than zero otherwise
 */
int free_arg_list(char** argv);

#endif // __ARG_UTIL_H__
