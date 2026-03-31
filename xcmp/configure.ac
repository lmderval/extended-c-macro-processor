AC_INIT([xcmp], [0.0.0], [louismaelderval@gmail.com])

AM_INIT_AUTOMAKE([subdir-objects] [foreign])

AM_SILENT_RULES([yes])

AM_PROG_AR

LT_INIT

AC_PROG_CXX

AX_COMPILER_FLAGS([], [], [], [-std=c++23 -Werror -Wall -Wextra -Wold-style-cast])

AC_CHECK_TOOL([CAT], [cat], [:])
AC_CHECK_TOOL([SED], [sed], [:])
AC_CHECK_TOOL([XCMGEN], [xcmgen], [:])

AC_PROG_LEX

AC_PROG_YACC

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/ast/Makefile
    src/ast/gen/Makefile
    src/parse/Makefile
    src/parse/gen/Makefile
    src/parse/tests/Makefile
    src/process/Makefile
    src/misc/Makefile
])

AC_OUTPUT
