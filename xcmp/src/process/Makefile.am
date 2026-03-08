lib_LTLIBRARIES = libprocess.la

libprocess_la_SOURCES = \
	process.hh \
	process.cc

libprocess_la_CPPFLAGS = -I$(top_srcdir)/src

libprocess_la_CXXFLAGS = -std=c++23 -Werror -Wall -Wextra -Wold-style-cast

libprocess_la_LIBADD = \
	$(top_srcdir)/src/ast/libast.la \
	$(top_srcdir)/src/parse/libparse.la
