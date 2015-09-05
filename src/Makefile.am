CFLAGS := -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
CFLAGS += -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wredundant-decls -Wnested-externs -Winline -Wno-long-long
CFLAGS += -Wconversion -Wstrict-prototypes -Wall -Werror -std=c99
CFLAGS += -D_GNU_SOURCE -D_DEFAULT_SOURCE -D_XOPEN_SOURCE -static -g -O0

lib_LIBRARIES = libdirutils.a
include_HEADERS = dirutils.h
dist_noinst_HEADERS = dirlist.h
libdirutils_a_SOURCES = dirutils.c dirlist.c dirwalk.c dirlist.h


