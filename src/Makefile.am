## Makefile.am for the ddebuglib

AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = ddebug_example
lib_LTLIBRARIES = libddebug.la

ddebug_example_SOURCES = \
	example.c
ddebug_example_LDADD = -lddebug
ddebug_example_CFLAGS = -D_DEBUG

libddebug_la_SOURCES = \
	debug.c \
	dlog.c \
	synchro.c \
	dio.c
libddebug_la_LIBADD = -lpthread

library_includedir=$(includedir)/ddebug
library_include_HEADERS = debuglib.h strlens.h debug.h dlog.h synchro.h system.h cpuinfo.h filechange.h array3.h linkedlist.h dio.h

INCLUDES =

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE_NAME@.pc
