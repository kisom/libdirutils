AM_LDFLAGS = -L/usr/local/include -lcunit
AM_CFLAGS = -Wall -g -I/usr/local/include -I../src -O0 
DIRLISTSRC = ../src/dirlist.c ../src/dirutils.c

check_PROGRAMS = dirlist_test dirutils_test
dirlist_test_CFLAGS = $(AM_CFLAGS) -DTEST_SUITE="\"dirlist_test\""
dirlist_test_SOURCES = ../src/dirlist.c dirlist_test.c 
dirutils_test_CFLAGS = $(AM_CFLAGS) -DTEST_SUITE="\"dirutils_test\""
dirutils_test_SOURCES = $(DIRLISTSRC) dirutils_test.c

