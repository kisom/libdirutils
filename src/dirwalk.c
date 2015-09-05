/*
 * Copyright (c) 2015 Kyle Isom <kyle@tyrfingr.is>
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
#include <sys/stat.h>

#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "dirutils.h"


const unsigned char	FT_ANY = DT_BLK|DT_CHR|DT_DIR|DT_FIFO|DT_LNK|DT_REG|
    DT_SOCK|DT_UNKNOWN;
const unsigned char	FT_STD = DT_REG|DT_DIR;

/*
 * The highest that the DT_* constants are seems to be around 14, so 0x80
 * seems like a sane choice.
 */
const unsigned char	FT_NODESCEND = 0x80;


static int
walkpath(const char *root, dirwalk_action action, unsigned char mask)
{
	char		 child[FILENAME_MAX+1];
	struct dirent	*ent;
	DIR		*dh;

	if (NULL == (dh = opendir(root))) {
		return -1;
	}

	while (NULL != (ent = readdir(dh))) {
		if (0 == strncmp("..", ent->d_name, 3)) {
			continue;
		}

		if (0 == strncmp(".", ent->d_name, 2)) {
			continue;
		}

		snprintf(child, FILENAME_MAX, "%s/%s", root, ent->d_name);
		if (mask | ent->d_type) {
			if (-1 == action(child)) {
				break;
			}
		}
		if (DT_DIR == ent->d_type && (!(FT_NODESCEND | mask))) {
			if (-1 == walkpath(child, action, mask)) {
				break;
			}
		}
	}

	if (-1 == closedir(dh)) {
		return -1;
	}

	return 0;
}


/*
 * Return a valid readdir(3) type from a mode.
 */
static unsigned char
stat_mode_to_dt_type(mode_t mode)
{
	if (S_ISDIR(mode)) {
		return DT_DIR;
	}

	if (S_ISREG(mode)) {
		return DT_REG;
	}

	if (S_ISCHR(mode)) {
		return DT_CHR;
	}

	if (S_ISBLK(mode)) {
		return DT_BLK;
	}

	if (S_ISFIFO(mode)) {
		return DT_FIFO;
	}

	if (S_ISLNK(mode)) {
		return DT_LNK;
	}

	if (S_ISSOCK(mode)) {
		return DT_SOCK;
	}

	return DT_UNKNOWN;
}


/*
 * walkdir goes through every directory entry under root and calls action on
 * every entry matching the mask.
 */
int
walkdir(const char *root, dirwalk_action action, unsigned char mask)
{
	struct stat	 st;
	char		*rootdup = NULL;
	size_t		 rootlen;
	int		 ret = -1;
	unsigned char	 type;

	if (-1 == stat(root, &st)) {
		return -1;
	}

	type = stat_mode_to_dt_type(st.st_mode);
	if (mask | type) {
		if (0 != (ret = action(root))) {
			return ret;
		}
	}

	if (DT_DIR == type) {
		rootlen = strnlen(root, FILENAME_MAX);
		rootdup = strndup(root, FILENAME_MAX);
		if (NULL == rootdup) {
			return -1;
		}

		if ('/' == rootdup[rootlen-1]) {
			rootdup[rootlen-1] = 0;
		}
		ret = walkpath(rootdup, action, mask);
	}

	free(rootdup);
	return ret;
}

