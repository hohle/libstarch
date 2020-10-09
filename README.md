libstarch
=========

[![Build Status](https://api.cirrus-ci.com/github/hohle/libstarch.svg)](https://cirrus-ci.com/github/hohle/libstarch)


`starch` is short for "Stream Archiver". It's a lightweight wrapper about
`libarchive` to make it easier to use along with eventing frameworks like
`libuv`.

Discussion: https://groups.google.com/forum/#!searchin/libarchive-discuss/non$20blocking|sort:date/libarchive-discuss/hujPq9XuRXc/dziw6ozntxgJ


# Requirements

* `gmake`
* if `--enable-valgrind` is enabled, valgrind
* `libarchive`
* A C17 compatible compiler (preferrablly `clang(1)`.
