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


Generating Makefiles
--------------------

Alternatively, it is possible to re-generate the Makefiles for the current
platform with `configure(1)` from the DeforaOS configure project, found at
<http://www.defora.org/os/project/16/configure>. The procedure is then as
follows:

    $ configure
    $ make

Please refer to the documentation of DeforaOS configure for further
instructions.

A default set of Makefiles is provided with this project in order to avoid a
circular dependency, as DeforaOS configure depends on libSystem itself. The
Makefiles provided target the NetBSD Operating System <http://www.netbsd.org/>
by default.


Python bindings
---------------

Part of libSystem's API can also be accessed from within the Python programming
language. This binding is not compiled or installed by default though. After
installing libSystem as documented above, the Python interface can be built and
installed as follows:

    $ (cd src/python && make install)

Likewise, an alternate installation directory may be specified:

    $ (cd src/python && make PREFIX="/path/to/libSystem" install)

Makefiles can be re-generated for the bindings as well:

    $ configure src/python
