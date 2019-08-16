TESTS = check_tar_gz
check_PROGRAMS = check_tar_gz

@CODE_COVERAGE_RULES@
@VALGRIND_CHECK_RULES@

check_tar_gz_SOURCES = check_tar_gz.c $(top_builddir)/src/starch.h
check_tar_gz_CFLAGS = $(CODE_COVERAGE_CFLAGS) @CHECK_CFLAGS@ -I$(top_builddir)/include
check_tar_gz_LDADD = $(top_builddir)/src/libstarch.la @CHECK_LIBS@

clean-local: code-coverage-clean
distclean-local: code-coverage-dist-clean
