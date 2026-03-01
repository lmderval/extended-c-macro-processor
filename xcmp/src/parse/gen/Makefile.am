BUILT_SOURCES = \
	lexer.cc \
	parser.hh \
	parser.cc \
	location.hh

AM_YFLAGS = -Wno-yacc

CLEANFILES = \
	lexer.cc \
	parser.hh \
	parser.cc \
	location.hh

noinst_LIBRARIES = libparse-gen.a

libparse_gen_a_CPPFLAGS = -I$(top_srcdir)/src

libparse_gen_a_CXXFLAGS = -std=c++20 -Werror -Wall -Wextra

libparse_gen_a_SOURCES = \
	lexer.ll \
	parser.yy

parser.hh: parser.cc
