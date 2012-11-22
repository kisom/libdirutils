Introduction
============

`libdirutils` is a utility library inspired by similar functionality in
the Go and Python programming languages; it provides functions similar
to `rm -r` and `mkdir -p`, as well as a convenience function to combine
the functionality of `access(2)` and `stat(2)`.

Functions Provided
==================

makedirs
--------

    int
    makedirs(const char *path);

`makedirs` creates a path and all required parent directories. The mode
created is the process’s `umask(2)` value applied to `0777`. It returns
`EXIT_SUCCESS` on success and `EXIT_FAILURE` on failure.

rmdirs
------

    int
    rmdirs(const char *path);

`rmdirs` removes a path and all subdirectories and files. It returns
`EXIT_SUCCESS` on success and `EXIT_FAILURE` on failure.

path\_exists
------------

    EXISTS_STATUS
    path_exists(const char *path);

`path_exists` combines the functionality of `access(2)` and `stat(2)`.
It checks whether the process has access to the file and indicates
whether it is a regular file, a directory, or if an error occurs. The
return type is of the enumeration `EXISTS_STATUS`, which is one of

*   `EXISTS_ERROR`: there was an error looking up the file;

*   `EXISTS_NOENT`: the file does not exist;

*   `EXISTS_NOPERM`: the process does not have the appropriate
    permissions to access the file.

*   `EXISTS_DIR`: the file is a direcotry;

*   `EXISTS_FILE`: the file is a regular file; and

*   `EXISTS_OTHER`: the file could be read and is not a directory or
    regular file.

Getting the Source
==================

Dependencies
------------

If you will be running the unit tests, `CUnit`[^1] is required. If you
will be rebuilding the autotools build infrastructure, you will need
automake 1.11 and autoconf >= 2.59.

Development Repository
----------------------

The source code repository is available on Github[^2]. The code there is
in the original autotools source; the `autobuild.sh` script is provided
to run through a complete build, including running through the provided
unit tests:

    $ git clone https://github.com/kisom/libdirutils.git dirutils
    $ cd dirutils
    $ ./autobuild.sh

Several utility scripts are provided in the `scripts` subdirectory; the
`autobuild.sh` script calls these. Of note, the `prebuild.sh` script,
which will set the necessary autotools environment variables and call
`autoreconf`.

Release Tarballs
----------------

Release tarballs will be made available via the Github repository’s
downloads[^3] section.

Once unpacked, these can be built with the normal

        ./configure && make && make install

method. See

        ./configure --help

for additional configuration options.

License
=======

    Copyright (c) 2012 Kyle Isom <kyle@tyrfingr.is>

    Permission to use, copy, modify, and distribute this software for any
    purpose with or without fee is hereby granted, provided that the above 
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 

[^1]: http://cunit.sourceforge.net/

[^2]: https://github.com/kisom/libdirutils

[^3]: https://github.com/kisom/libdirutils/downloads
