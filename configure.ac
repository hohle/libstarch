# Copyright (C) 2019 Jonathan Hohle
# Autoconf/make configuration for libstarch

AC_PREREQ(2.60)
AC_INIT(libstarch, 0.0.1)

AC_PACKAGE_URL(https://github.com/hohle/libstarch)
AC_COPYRIGHT([Copyright © 2019 Jonathan Hohle])

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR(build-aux) # should this be in build?
AC_CONFIG_LIBOBJ_DIR(libaco) # wat?
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects dist-xz])

AC_CANONICAL_HOST

: ${CLFAGS="-I/usr/local/include -I/opt/local/include"}
AC_LANG(C)
AC_PROG_CC(clang llvm-gcc gcc)
AC_PROG_CPP
AC_PROG_INSTALL
AM_PROG_AR
AM_PROG_AS
LT_INIT

if test "x$enable_code_coverage" = "xyes"; then
    AC_CHECK_TOOL(HAS_LLVM_COV, llvm-cov, ":")
    if test "x$HAS_LLVM_COV" != "x:"; then
        GCOV="$PWD/llvm-gcov"
    fi
fi

#AX_CODE_COVERAGE
#AX_VALGRIND_DFLT([drd], [off])
#AX_VALGRIND_DFLT([helgrind], [off])
#AX_VALGRIND_DFLT([memcheck], [on])
#AX_VALGRIND_DFLT([sgcheck], [on])
#AX_VALGRIND_CHECK

PKG_CHECK_MODULES([CHECK], [check >= 0.10.0])

AC_HEADER_STDC
AC_CHECK_HEADERS([ \
    archive.h \
    execinfo.h \
    getopt.h \
    stdbool.h \
    stdio.h \
    stdlib.h \
    string.h \
    time.h \
    unistd.h \
    limits.h \
    stddef.h \
])
AC_CHECK_HEADER_STDBOOL

AC_CHECK_FUNCS([ \
    strdup \
    strndup \
    strstr \
    memset \
    munmap \
])
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_ARG_ENABLE(debug,
              AC_HELP_STRING(--enable-debug, create a debug build),
              found_debug=$enable_debug)
AM_CONDITIONAL(IS_DEBUG, test, "x$found_debug" = xyes)

AC_EGREP_CPP(yes, [
        #ifdef __clang__
        yes
        #endif
    ],
    found_clang=yes,
    found_clang=no)
AM_CONDITIONAL(IS_CLANG, test "x$found_clang" = xyes)

AC_CHECK_LIB(archive, archive_read_open)
AC_SEARCH_LIBS(malloc, m)

AC_EGREP_CPP(yes, [
        #include <features.h>
        #ifdef __GLIBC__
        yes
        #endif
    ],
    found_glibc=yes,
    found_glibc=no)
AM_CONDITIONAL(IS_GLIBC, test "x$found_glibc" = xyes)

# todo: probably unnecessary
XOPEN_DEFINES=
AC_SUBST(XOPEN_DEFINES)

# man format defaults to mdoc
MANFORMAT=mdoc
AC_SUBST(MANFORMAT)

AC_MSG_CHECKING(platform)
case "$host_os" in
    *darwin*)
        AC_MSG_RESULT(darwin)
        PLATFORM=darwin
        AC_CHECK_PROG(found_macports, port, yes)
        AC_CHECK_PROG(found_homebrew, brew, yes)
        ;;
    *linux*)
        AC_MSG_RESULT(linux)
        PLATFORM=linux
        DEP_GMAKE=make
        AC_CHECK_PROG(found_yum, yum, yes)
        AC_CHECK_PROG(found_apt, apt, yes)
        ;;
    *freebsd*)
        AC_MSG_RESULT(freebsd)
        PLATFORM=freebsd
        AC_CHECK_PROG(found_pkg, pkg, yes)
        ;;
    *)
        AC_MSG_RESULT(unknown)
        PLATFORM=unknown
        ;;
esac
AC_SUBST(PLATFORM)
AM_CONDITIONAL(IS_DARWIN, test "x$PLATFORM" = xdarwin)
AM_CONDITIONAL(IS_LINUX, test "x$PLATFORM" = xlinux)
AM_CONDITIONAL(IS_FREEBSD, test "x$PLATFORM" = xfreebsd)
AM_CONDITIONAL(IS_UNKNOWN, test "x$PLATFORM" = xunknown)

DEP_ARCHIVE=libarchive
DEP_GMAKE=gmake
case "yes" in
    "$found_macports")
        PACKAGE_MANAGER=macports
        ;;
    "$found_homebrew")
        PACKAGE_MANAGER=homebrew
        ;;
    "$found_macports")
        PACKAGE_MANAGER=apt
        DEP_CURL=libcurl-dev
        DEP_UV=libuv-dev
        DEP_JSONC=json-c-dev
        ;;
    "$found_yum")
        PACKAGE_MANAGER=yum
        ;;
    "$found_apt")
        PACKAGE_MANAGER=apt
        ;;
    "$found_pkg")
        PACKAGE_MANAGER=pkg
        ;;
    *)
        PACKAGE_MANAGER=unknown
        ;;
esac
DEPENDENCIES="$DEP_ARCHIVE $DEP_GMAKE"
AC_SUBST(PACKAGE_MANAGER)
AC_SUBST(DEPENDENCIES)

AC_CONFIG_FILES([ \
    Makefile \
    libaco/Makefile \
    src/Makefile \
    test/Makefile \
])

AC_OUTPUT
