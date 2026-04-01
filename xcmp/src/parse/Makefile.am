SUBDIRS = \
	. \
	tests

lib_LTLIBRARIES = libparse.la

include gen/local.am

libparse_la_SOURCES = \
	lexer.hh \
	driver.hh \
	driver.hxx \
	driver.cc

libparse_la_SOURCES += $(GENERATED)

libparse_la_CPPFLAGS = -I$(top_srcdir)/src

libparse_la_CXXFLAGS = -std=c++23 -Werror -Wall -Wextra -Wno-maybe-uninitialized

libparse_la_LIBADD = \
	$(top_srcdir)/src/ast/libast.la
