MAINTAINERCLEANFILES = Makefile.in

#noinst_PROGRAMS = filter_dup
noinst_HEADERS = linebreak.h linebreakdef.h wordbreakdef.h  wordbreak.h
noinst_LTLIBRARIES = liblinebreak.la

liblinebreak_la_SOURCES = \
	linebreak.c \
	linebreakdata.c \
	linebreakdef.c \
	wordbreak.c \
	wordbreakdata.x

EXTRA_DIST = \
	LineBreak1.sed \
	LineBreak2.sed \
	linebreakdata1.tmpl \
	linebreakdata2.tmpl \
	linebreakdata3.tmpl \
	LICENCE
