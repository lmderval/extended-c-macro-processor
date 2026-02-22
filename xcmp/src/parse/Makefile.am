SUBDIRS = \
	. \
	tests

BUILT_SOURCES = \
	lexer.cc

lib_LTLIBRARIES = libparse.la

libparse_la_SOURCES = \
	lexer.ll

libparse_la_CPPFLAGS = -I$(top_srcdir)/src

libparse_la_CXXFLAGS = -std=c++20 -Werror -Wall -Wextra

CLEANFILES = \
	lexer.cc

lexer.cc: lexer.ll
	$(AM_V_LEX)$(am__skiplex) $(SHELL) $(YLWRAP) $< $(LEX_OUTPUT_ROOT).cc $@ -- $(LEXCOMPILE)
