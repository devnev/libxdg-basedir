#!/bin/sh

autoreconf --force --install
./configure "$@"
