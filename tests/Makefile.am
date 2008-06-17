AM_CFLAGS = -I$(top_srcdir)/include

check_PROGRAMS = testdump testfind

TESTS = testdump

testdump_SOURCES = testdump.c
testdump_LDFLAGS = $(all_libraries)
testdump_LDADD = ../src/libxdg-basedir.la

testfind_SOURCES = testfind.c
testfind_LDFLAGS = $(all_libraries)
testfind_LDADD = ../src/libxdg-basedir.la
