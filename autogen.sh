#!/bin/sh

# This script re-generates the "configure" script and associated files,
# from the configure.ac (autoconf) source.

autoreconf --force --install || exit 1
