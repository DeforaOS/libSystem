DeforaOS libSystem
==================

About libSystem
---------------

libSystem is the most essential library within DeforaOS, with the aim to become
a complete C runtime (thus obsoleting the libc and replacing its API).
Regardless of its progress in this regard, libSystem is intended to be portable
and work on any system, using any POSIX-compliant runtime as fallback if
available.


Compiling libSystem
-------------------

With GCC, this should be enough:

    $ make

To install libSystem in a dedicated directory, like `/path/to/libSystem`:

    $ make PREFIX="/path/to/libSystem" install


Platform-specific notes
-----------------------

On some GNU/Linux distributions, it may be necessary to link libSystem to libdl
explicitly:

    $ make LDFLAGS="-ldl"


Python bindings
---------------

Part of libSystem's API can also be accessed from within the Python programming
language. This binding is not compiled or installed by default though. After
installing libSystem as documented above, the Python interface can be built and
installed as follows:

    $ (cd src/python && make install)

Likewise, an alternate installation directory may be specified:

    $ (cd src/python && make PREFIX="/path/to/libSystem" install)
