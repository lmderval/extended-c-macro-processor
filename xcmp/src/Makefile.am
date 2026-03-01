SUBDIRS = \
	parse/gen \
	ast \
	parse \
	process

bin_PROGRAMS = xcmp

xcmp_SOURCES = main.cc

xcmp_CPPFLAGS = -I%D%

xcmp_CXXFLAGS = -std=c++20 -Werror -Wall -Wextra -Wold-style-cast

xcmp_LDADD = \
	ast/libast.la \
	parse/libparse.la \
	process/libprocess.la
