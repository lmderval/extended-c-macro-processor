SUBDIRS = \
	. \
	tests

BUILT_SOURCES = \
	lexer.cc \
	parser.hh \
	parser.cc \
	location.hh

AM_YFLAGS = -Wno-yacc

lib_LTLIBRARIES = libparse.la

libparse_la_SOURCES = \
	lexer.hh \
	lexer.ll \
	parser.yy

libparse_la_CPPFLAGS = -I$(top_srcdir)/src

libparse_la_CXXFLAGS = -std=c++20 -Werror -Wall -Wextra

CLEANFILES = \
	lexer.cc \
	parser.hh \
	parser.cc \
	location.hh

libparse_la_LIBADD = \
	$(top_srcdir)/src/ast/libast.la

lexer.cc: lexer.ll
	$(AM_V_LEX)$(am__skiplex) $(SHELL) $(YLWRAP) $< $(LEX_OUTPUT_ROOT).cc $@ -- $(LEXCOMPILE)

parser.hh: parser.cc
