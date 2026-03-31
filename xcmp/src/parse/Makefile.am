SUBDIRS = \
	. \
	tests

BUILT_SOURCES = \
	lexer.cc \
	parser.hh \
	parser.cc

CLEANFILES = \
	lexer.cc \
	parser.hh \
	parser.cc

lib_LTLIBRARIES = libparse.la

libparse_la_SOURCES = \
	lexer.hh \
	lexer.cc \
	parser.hh \
	parser.cc \
	location.hh \
	driver.hh \
	driver.hxx \
	driver.cc

libparse_la_CPPFLAGS = -I$(top_srcdir)/src

libparse_la_CXXFLAGS = -std=c++23 -Werror -Wall -Wextra

libparse_la_LIBADD = \
	$(top_srcdir)/src/ast/libast.la

lexer.cc: gen/lexer.cc
	$(AM_V_GEN)$(CP) $< $@

parser.hh: gen/parser.hh
	$(AM_V_GEN)$(CP) $< $@

parser.cc: gen/parser.cc
	$(AM_V_GEN)$(CP) $< $@
