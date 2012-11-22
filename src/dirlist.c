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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dirlist.h"


/*
 * create and initialise the directory list.
 */
struct tq_dirlst
*dirlst_create(const char *init_path, size_t init_path_sz)
{
        struct tq_dirlst        *path_lst;
        struct dirlst           *elm;

        if (init_path_sz > FILENAME_MAX)
                return NULL;
        path_lst = calloc(1, sizeof(struct tq_dirlst));
        if (NULL == path_lst)
                return path_lst;
        elm = calloc(1, sizeof(struct dirlst));
        if (NULL == elm) {
                free(path_lst);
                return NULL;
        }
        memcpy(elm->path, init_path, init_path_sz);

        TAILQ_INIT(path_lst);
        TAILQ_INSERT_HEAD(path_lst, elm, dirs);

        return path_lst;
}


/*
 * add a new directory to the list.
 */
int
dirlst_push(struct tq_dirlst *lst, const char *new_dir, size_t new_dir_sz)
{
        struct dirlst   *elm;

        if (new_dir_sz > FILENAME_MAX)
                return EXIT_FAILURE;
        elm = calloc(1, sizeof(struct dirlst));
        if (NULL == elm)
                return EXIT_FAILURE;
        strncpy(elm->path, new_dir, FILENAME_MAX);
        TAILQ_INSERT_HEAD(lst, elm, dirs);
        return EXIT_SUCCESS;
}


/*
 * remove the first directory in the list and return it
 */
struct dirlst
*dirlst_pop(struct tq_dirlst *lst)
{
        struct dirlst   *elm;

        if (TAILQ_EMPTY(lst))
                return NULL;
        elm = TAILQ_FIRST(lst);
        TAILQ_REMOVE(lst, elm, dirs);
        return elm;
}


int
dirlst_destroy(struct tq_dirlst **lstp)
{
        struct dirlst   *elm;

        while ((elm = TAILQ_FIRST(*lstp))) {
                TAILQ_REMOVE(*lstp, elm, dirs);
                free(elm);
        }
        if (!TAILQ_EMPTY(*lstp))
                return EXIT_FAILURE;
        free(*lstp);
        *lstp = NULL;
        return EXIT_SUCCESS;
}
