mkdir -p build-aux # this should be something else
libtoolize
aclocal
automake --add-missing --force-missing --copy --foreign
autoreconf
