AC_INIT([xcmp], [0.0.0], [louismaelderval@gmail.com])

AM_INIT_AUTOMAKE([subdir-objects] [foreign])

AM_SILENT_RULES([yes])

AM_PROG_AR

LT_INIT

AC_PROG_CXX

AX_COMPILER_FLAGS([], [], [], [-std=c++20 -Werror -Wall -Wextra -Wold-style-cast])

AC_CHECK_TOOL([CP], [cp], [:])

AC_PROG_LEX

AC_PROG_YACC

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/ast/Makefile
    src/parse/Makefile
    src/parse/gen/Makefile
    src/parse/tests/Makefile
    src/process/Makefile
])

AC_OUTPUT
