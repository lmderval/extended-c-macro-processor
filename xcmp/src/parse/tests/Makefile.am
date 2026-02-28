bin_PROGRAMS = \
	lex \
	parse

lex_SOURCES = lex.cc

parse_SOURCES = parse.cc

AM_CPPFLAGS = -I$(top_srcdir)/src

AM_CXXFLAGS = -std=c++20 -Werror -Wall -Wextra -Wold-style-cast

LDADD = \
	$(top_srcdir)/src/parse/libparse.la
