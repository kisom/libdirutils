/*
 * Copyright (c) 2012 Kyle Isom <kyle@tyrfingr.is>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * ---------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/stat.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dirutils.h"
#include "dirlist.h"


/*
 * Determines whether a directory exists.
 */
EXISTS_STATUS
path_exists(const char *path, size_t pathlen)
{
        struct stat     st;
        int             rv;

        rv = stat(path, &st);
        if (rv == -1) {
                switch (errno) {
                case EACCES:
                        return EXISTS_NOPERM;
                        break;
                case ENOENT:
                        return EXISTS_NOENT;
                        break;
                default:
                        return EXISTS_ERROR;
                }
        }

        if (st.st_mode & S_IFDIR)
                return EXISTS_DIR;
        else if (st.st_mode & S_IFREG)
                return EXISTS_FILE;
        else
                return EXISTS_ERROR;
}


/*
 * create a path and any directories above it required
 */
int
makedirs(const char *path, size_t path_sz)
{
        struct tq_dirlst   *lst;

        lst = dirlst_create(path, path_sz);
        if (NULL == lst)
                return EXIT_FAILURE;

        return EXIT_FAILURE;
}


/*
 * remove a directory and any subdirectories
 */
int
rmdirs(const char *, size_t);

