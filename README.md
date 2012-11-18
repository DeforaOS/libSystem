DeforaOS libSystem
==================

About libSystem
---------------

libSystem is the most essential library within DeforaOS, with the aim to become
a complete C runtime (thus obsoleting the libc and replacing its API).
Regardless of its progress in this regard, libSystem is intended to be portable
and work on any system, using any POSIX-compliant runtime provided instead.

Compiling libSystem
-------------------

With GCC, this should be enough:

    $ make

To install libSystem in a dedicated directory, like `/path/to/libSystem`:

    $ make PREFIX="/path/to/libSystem" install

