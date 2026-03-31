SUBDIRS = \
	misc \
	ast/gen \
	parse/gen \
	ast \
	parse \
	process

bin_PROGRAMS = xcmp

xcmp_SOURCES = main.cc

xcmp_CPPFLAGS = -I%D%

xcmp_CXXFLAGS = -std=c++23 -Werror -Wall -Wextra -Wold-style-cast

xcmp_LDADD = \
	misc/libmisc.la \
	ast/libast.la \
	parse/libparse.la \
	process/libprocess.la
