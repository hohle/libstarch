lib_LTLIBRARIES = libstarch.la

AM_LIBS = $(CODE_COVERAGE_LIBS)

AM_CFLAGS = \
	$(CODE_COVERAGE_CFLAGS) \
	-std=gnu17 \
        -Wpointer-arith \
        -Wstack-protector \
        -fexceptions \
        -fstack-protector \
        --param=ssp-buffer-size=4  \
        -std=gnu11 \
        -m64 \
        -mtune=generic \
        -pipe

AM_CFLAGS += \
	-Wconversion \
	-Wused-but-marked-unused \
	-Wgcc-compat \
	-Wold-style-cast \
	-pedantic-errors \
        -Wall \
        -Werror \
        -Wextra \
        -Wunreachable-code \
	-Wno-gnu

AM_CFLAGS += \
        -Wp,-D_FORTIFY_SOURCE=2


libstarch_la_SOURCES = starch.c
libstarch_la_CFLAGS = $(AM_CFLAGS) -I../include -I../libaco
libstarch_la_LDFLAGS = -larchive
libstarch_la_LIBADD = $(top_builddir)/libaco/libaco.la
