mkdir -p build-aux # this should be something else

case `uname` in
  Darwin*)
      glibtoolize --copy ;;
  *)
      libtoolize --copy ;;
esac

aclocal
automake --add-missing --force-missing --copy --foreign
autoreconf
