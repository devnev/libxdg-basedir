# libxdg-basedir

This is my fork of **libxdg-basedir**, by Mark Nevill. The original appears to be unmaintained.

I have fixed an obvious bug and performed minor cleanup up the code. More importantly, I
intend to issue new releases when fixes are made. I cannot attest as to the quality but
will accept pull requests containing bug fixes.

## The XDG Base Directory specification

**libxdg-basedir** is "An implementation of the XDG Base Directory specifications".
[These specifications](https://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html)
set a standard for where applications should find/store preferences, cache files, and other
run-time files/data, mostly based on the values of certain environment variables.

I'm not a huge fan of the specifications: the arbitrary requirement that timestamps on files be
updated periodically to avoid cleanup places an unnecessary burden on applications, and the
workaround of setting the sticky bit is an abuse of a defunct feature. However, having a way of
specifying where certain kinds of files should be placed is useful, and applications do make
use of the specification (and this library).

## Library documentation

There appears to be doxygen-based comments, but I have not attempted to build documentation.

## Building

The library uses autotools so can usually be built and installed via an invocation similar to:

    ./configure --prefix=/usr
    make
    make install

You can install to an alternate root location (for creation of packages etc) using
`make DESTDIR=/some/directory install`. Use `./configure --help` for available configuration
options.
