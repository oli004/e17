MAINTAINERCLEANFILES = Makefile.in .sources-ts

AM_CPPFLAGS = -DPKGDATADIR=\"$(pkgdatadir)\" \
		-I ../			\
		@PTHREAD_CFLAGS@	\
		@GLIB_CFLAGS@		\
		@SQLITE_CFLAGS@		\
		@AZY_CFLAGS@	\
		`curl-config --cflags`	\
		@ECORE_X_CFLAGS@	\
		@ELEMENTARY_CFLAGS@	\
		-DLOCALEDIR=\"$(localedir)\" \
		-Wall


AM_LDFLAGS =	@PTHREAD_LIBS@		\
		@GLIB_LIBS@		\
		@SQLITE_LIBS@		\
		@AZY_LIBS@		\
		`curl-config --libs`	\
		@ECORE_X_LIBS@ \
		@ELEMENTARY_LIBS@

BUILT_SOURCES = statusnet_Common.h statusnet_Common.c statusnet_Common_Azy.h statusnet_Common_Azy.c
CLEANFILES = $(BUILT_SOURCES) .sources-ts
$(BUILT_SOURCES): .sources-ts
.sources-ts: statusnet.azy
	azy_parser -eHn -m common-impl,common-headers -o $(top_builddir)/src $(top_builddir)/src/statusnet.azy
	touch $(top_builddir)/src/.sources-ts

bin_PROGRAMS = elmdentica

elmdentica_SOURCES =	\
	$(BUILT_SOURCES) \
	curl.c	\
	statusnet.c	\
	settings.c	\
	elmdentica.c

EXTRA_DIST=		\
	statusnet.azy	\
	gettext.h	\
	curl.h		\
	statusnet.h	\
	settings.h	\
	elmdentica.h
