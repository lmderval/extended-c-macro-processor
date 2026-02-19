AC_INIT([xcmp], [0.0.0], [louismaelderval@gmail.com])

AM_INIT_AUTOMAKE([subdir-objects] [foreign])

AM_SILENT_RULES([yes])

AM_PROG_AR

AC_PROG_RANLIB

AC_PROG_CXX

AX_COMPILER_FLAGS([], [], [], [-std=c++20 -Werror -Wall -Wextra -Wold-style-cast])

AC_CONFIG_FILES([
    Makefile
	src/Makefile
])

AC_OUTPUT
