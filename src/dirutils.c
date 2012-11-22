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

#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dirutils.h"
#include "dirlist.h"


static int      _parent_exists(const char *);
static int      _rmdirs(const char *);

/*
 * Determines whether a directory exists.
 */
EXISTS_STATUS
path_exists(const char *path)
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
                return EXISTS_OTHER;
}


/*
 * create a path and any directories above it required
 */
int
makedirs(const char *path)
{
        struct tq_dirlst        *lst;
        struct dirlst           *elm;
        size_t                  path_sz;
        char                    *dnam_p, *curpath;

        path_sz = strlen(path);
        lst = dirlst_create(path, path_sz);
        if (NULL == lst)
                return EXIT_FAILURE;

        curpath = strdup(path);
        while (!_parent_exists(curpath)) {
                dnam_p = dirname(curpath);
                free(curpath);
                curpath = strdup(dnam_p);
                dirlst_push(lst, curpath, strlen(curpath));
        }
        free(curpath);

        while (NULL != (elm = dirlst_pop(lst))) {
                if (-1 == mkdir(elm->path, 0777)) {
                        free(elm);
                        return EXIT_FAILURE;
                }
                free(elm);
        }

        return dirlst_destroy(&lst);
}


/*
 * remove a directory and any subdirectories
 */
int
rmdirs(const char *path)
{
        return _rmdirs(path);
}


/*
 * Determine whether the parent directory exists, and return true if it
 * does.
 */
int
_parent_exists(const char *path)
{
        char    *name_buf;
        char    *dnam_p;
        size_t   path_len;

        name_buf = strdup(path);

        dnam_p = dirname(name_buf);
        path_len = strlen((char *)dnam_p);
        free(name_buf);

        if (EXISTS_DIR != path_exists(dnam_p))
                return 0;
        else
                return 1;
}


/*
 * remove a directory, all files in it, and all subdirectories.
 */
int
_rmdirs(const char *path)
{
        char             child[FILENAME_MAX + 1];
        struct dirent   *dp;
        DIR             *dirp;
        int              fail;

        if (NULL == (dirp = opendir(path)))
                return EXIT_FAILURE;
        while (NULL != (dp = readdir(dirp))) {
                if (0 == strncmp("..", dp->d_name, 3))
                        continue;
                if (0 == strncmp(".", dp->d_name, 2))
                        continue;
                snprintf(child, FILENAME_MAX, "%s/%s", path, dp->d_name);
                if (DT_DIR == dp->d_type) {
                        fail = _rmdirs(child);
                        if (EXIT_FAILURE == fail)
                                break;
                } else {
                        fail = unlink(child);
                        if (-1 == fail) {
                                fail = EXIT_FAILURE;
                                break;
                        }
                }
        }
        if (-1 == closedir(dirp))
                return EXIT_FAILURE;
        if (EXIT_FAILURE == fail)
                return EXIT_FAILURE;
        else if (-1 == rmdir(path))
                return EXIT_FAILURE;
        else
                return EXIT_SUCCESS;
}
