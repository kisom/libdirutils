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


#ifndef __DIRUTILS_DIRUTILS_H
#define __DIRUTILS_DIRUTILS_H


#include <sys/queue.h>
#include <dirent.h>
#include <stdio.h>

enum E_EXISTS_STATUS {
	EXISTS_ERROR,
	EXISTS_NOENT,
	EXISTS_NOPERM,
	EXISTS_DIR,
	EXISTS_FILE,
	EXISTS_OTHER
};
typedef enum E_EXISTS_STATUS EXISTS_STATUS;


typedef int (*dirwalk_action)(const char *);


extern const unsigned char	FT_ANY;
extern const unsigned char	FT_STD;
extern const unsigned char	FT_NODESCEND;


int		 makedirs(const char *);
int		 rmdirs(const char *);
EXISTS_STATUS	 path_exists(const char *);
int		 walkdir(const char *, dirwalk_action, unsigned char);


#endif
