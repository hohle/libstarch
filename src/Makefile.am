lib_LTLIBRARIES = libstarch.la

AM_LIBS = $(CODE_COVERAGE_LIBS)
AM_CFLAGS = $(CODE_COVERAGE_CFLAGS)

libstarch_la_SOURCES = starch.c
libstarch_la_CFLAGS = $(CODE_COVERAGE_CFLAGS) -I../include -I../libaco
libstarch_la_LDFLAGS = -larchive
libstarch_la_LIBADD = $(top_builddir)/libaco/libaco.la
