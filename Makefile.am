
AM_CFLAGS=-I$(top_srcdir)/src

# libdisarm.pc
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libdisarm.pc


# libsexp.la
LIBDISARMSOURCES = \
	src/libdisarm/args.c \
	src/libdisarm/parser.c \
	src/libdisarm/print.c

LIBDISARMHEADERS = \
	src/libdisarm/args.h \
	src/libdisarm/disarm.h \
	src/libdisarm/macros.h \
	src/libdisarm/parser.h \
	src/libdisarm/print.h \
	src/libdisarm/types.h

LIBDISARMPRIVHEADERS = \
	src/libdisarm/endian.h

lib_LTLIBRARIES = libdisarm.la

libdisarm_la_SOURCES = \
	$(LIBDISARMSOURCES) \
	$(LIBDISARMHEADERS) \
	$(LIBDISARMPRIVHEADERS)
pkginclude_HEADERS = $(LIBDISARMHEADERS)
noinst_HEADERS = $(LIBDISARMPRIVHEADERS)
libdisarm_la_LDFLAGS = -version-info $(LIBDISARM_VERSION_INFO)


# dacli
bin_PROGRAMS = dacli

dacli_SOURCES = src/dacli/dacli.c
dacli_LDADD = libdisarm.la
