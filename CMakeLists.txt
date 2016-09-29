cmake_minimum_required (VERSION 3.6)
project (cli_decode)

# Set the install directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

# Set the sources
set(CLI_DECODE_SRCS
    src/cli_clone.c
    src/arg_util.c
	src/cli_file_util.c
	src/cli_import_dat.c
	src/cli_prefs_prf.c
	src/cli_misc.c
	src/cli_crypto.c
)

# Set include directories
include_directories(cli PUBLIC include)

# Add executable
add_executable(cli ${CLI_DECODE_SRCS})

install(TARGETS cli DESTINATION bin)
