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


#ifndef __DIRUTILS_DIRLIST_H
#define __DIRUTILS_DIRLIST_H


#include <stdio.h>
#include <dirent.h>
#include "queue.h"


struct dirlst {
        char                    path[FILENAME_MAX + 1];
        TAILQ_ENTRY(dirlst)     dirs;
};
TAILQ_HEAD(tq_dirlst, dirlst);


struct tq_dirlst	*dirlst_create(const char *, size_t);
int			 dirlst_push(struct tq_dirlst *, const char *, size_t);
struct dirlst		*dirlst_pop(struct tq_dirlst *);
int			 dirlst_destroy(struct tq_dirlst **);


#endif
