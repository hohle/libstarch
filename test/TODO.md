To Do:
======

Add tests for each of the libarchive supported formats:

>  •   old-style tar archives,
>  •   most variants of the POSIX “ustar” format,
>  •   the POSIX “pax interchange” format,
>  •   GNU-format tar archives,
>  •   most common cpio archive formats,
>  •   ISO9660 CD images (including RockRidge and Joliet extensions),
>  •   Zip archives,
>  •   ar archives (including GNU/SysV and BSD extensions),
>  •   Microsoft CAB archives,
>  •   LHA archives,
>  •   mtree file tree descriptions,
>  •   RAR archives,
>  •   XAR archives.
>
>  The library automatically detects archives compressed with gzip(1),
>  bzip2(1), xz(1), lzip(1), or compress(1) and decompresses them
>  transparently.  It can similarly detect and decode archives processed
>  with uuencode(1) or which have an rpm(1) header.
