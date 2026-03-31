lib_LTLIBRARIES = libast.la

include gen/local.am

libast_la_SOURCES = \
	all.hh \
	fwd.hh \
	ast.hh \
	ast.hxx \
	ast.cc \
	visitor.hh \
	visitor.hxx \
	pretty-printer.hh \
	pretty-printer.hxx \
	pretty-printer.cc

libast_la_SOURCES += $(GENERATED)

libast_la_CPPFLAGS = -I$(top_srcdir)/src

libast_la_CXXFLAGS = -std=c++23 -Werror -Wall -Wextra -Wold-style-cast
