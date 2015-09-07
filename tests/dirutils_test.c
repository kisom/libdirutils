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
#include <sys/stat.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include <dirutils.h>

static int      test_write_file_helper(const char *, const char *);
static int      test_touch_file_helper(const char *);

/*
 * test the use of the exists function
 */
void
test_exists(void)
{
        char            testdir[] = "testdata/testdir";
        char            testfil[] = "testdata/testfile";
        char            testnot[] = "testdata/nosuchfile";
        EXISTS_STATUS   ftype;

        ftype = path_exists(testdir);
        CU_ASSERT(EXISTS_DIR == ftype);

        ftype = path_exists(testfil);
        CU_ASSERT(EXISTS_FILE == ftype);

        ftype = path_exists(testnot);
        CU_ASSERT(EXISTS_NOENT == ftype);
}

void
test_makedirs(void)
{
        char     testpath[] = "testdata/foo/bar/baz\0";

        /*
         * use the system to ensure we have a clean slate for this test
         */
        if (EXISTS_DIR == path_exists(testpath))
                system("rm -fr testdata/foo/");
        CU_ASSERT(EXIT_SUCCESS == makedirs(testpath));
        CU_ASSERT(EXISTS_DIR == path_exists(testpath));
        /*
         * we can't guarantee rmdirs yet; this ensures a clean slate.
         */
        system("rm -r testdata/foo/");
}

void
test_empty_rmdirs(void)
{
        char    testpath[20] = "testdata/foo";
        char    cmd[FILENAME_MAX];
        int     rv;

        memset(cmd, 0x0, FILENAME_MAX);
        snprintf(cmd, FILENAME_MAX, "mkdir -p %s/bar/baz", testpath);
        system(cmd);
	rv = rmdirs(testpath);
	if (EXIT_FAILURE == rv) {
		printf("\n");
		warn("rmdirs");
		system("rm -fr testdata/foo/");
	}	
        CU_ASSERT(EXIT_SUCCESS == rv);
        CU_ASSERT(EXISTS_NOENT == path_exists(testpath));
        /*
         * we can't guarantee rmdirs yet; this ensures a clean slate.
         */
}


void
test_rmdirs_simple(void)
{
        char    testpath[] = "testdata/foo";
        char    cmd[FILENAME_MAX];
        int     rv;

        memset(cmd, 0x0, FILENAME_MAX);
        snprintf(cmd, FILENAME_MAX, "mkdir -p %s/bar/baz", testpath);
        system(cmd);
        memset(cmd, 0x0, FILENAME_MAX);
        snprintf(cmd, FILENAME_MAX, "touch %s/bar/quux", testpath);
        system(cmd);
	rv = rmdirs(testpath);
	if (EXIT_FAILURE == rv) {
		printf("\n");
		warn("rmdirs");
		/*
		 * we can't guarantee rmdirs yet; this ensures a clean slate.
		 */
		system("rm -r testdata/foo/ 2>/dev/null");
	}	
        CU_ASSERT(EXIT_SUCCESS == rv);
        CU_ASSERT(EXISTS_NOENT == path_exists(testpath));
}


void
test_dirutils(void)
{
        char    testpath[] = "testdata/dirutils";
        char    tmp_path[FILENAME_MAX + 1];
        char    lnk_path[FILENAME_MAX + 1];

        /* set up directory structure */
        CU_ASSERT(EXISTS_NOENT == path_exists(testpath));
        snprintf(tmp_path, FILENAME_MAX, "%s/foo/bar", testpath);
        CU_ASSERT(EXIT_SUCCESS == makedirs(tmp_path));
        CU_ASSERT(EXISTS_DIR == path_exists(tmp_path));
        snprintf(tmp_path, FILENAME_MAX, "%s/foo/baz", testpath);
        CU_ASSERT(EXIT_SUCCESS == makedirs(tmp_path));
        CU_ASSERT(EXISTS_DIR == path_exists(tmp_path));

        /* add a few files */
        snprintf(tmp_path, FILENAME_MAX, "%s/foo/quux", testpath);
        CU_ASSERT(EXIT_SUCCESS == test_touch_file_helper(tmp_path));
        CU_ASSERT(EXISTS_FILE == path_exists(tmp_path));
        snprintf(tmp_path, FILENAME_MAX, "%s/foo/bar/xyzzy", testpath);
        snprintf(lnk_path, FILENAME_MAX, "%s/foo/baz/xyzzy", testpath);
        CU_ASSERT(EXIT_SUCCESS == test_write_file_helper(tmp_path,
	    "some data should go here"));
        CU_ASSERT(EXISTS_FILE == path_exists(tmp_path));
        CU_ASSERT(-1 != link(tmp_path, lnk_path));
        CU_ASSERT(EXISTS_FILE == path_exists(lnk_path));
        snprintf(tmp_path, FILENAME_MAX, "%s/foo/bar/thud", testpath);
        snprintf(lnk_path, FILENAME_MAX, "%s/foo/baz/fred", testpath);
        CU_ASSERT(EXIT_SUCCESS == test_write_file_helper(tmp_path,
	    "we want something for the link"));
        CU_ASSERT(EXISTS_FILE == path_exists(tmp_path));
        CU_ASSERT(-1 != symlink(tmp_path, lnk_path));

        CU_ASSERT_FATAL(EXIT_SUCCESS == rmdirs(testpath));
        CU_ASSERT_FATAL(EXISTS_NOENT == path_exists(testpath));
}


/*
 * utility function to touch a file
 */
static int
test_write_file_helper(const char *path, const char *data)
{
        ssize_t wrsz;
        size_t  data_len;
        int     fail, fd;

        fail = EXIT_SUCCESS;
        data_len = strlen(data);
        fd = open(path, O_WRONLY|O_CREAT, S_IRUSR| S_IWUSR);
        if (-1 == fd)
                return EXIT_FAILURE;
        wrsz = write(fd, data, data_len);
        if (wrsz != data_len)
                fail = EXIT_FAILURE;
        if (-1 == close(fd))
                fail = EXIT_FAILURE;
        return fail;
}


static int
test_touch_file_helper(const char *path)
{
        return test_write_file_helper(path, "");
}

/*
 * Stubs required by the test suite, but for which no functionality is
 * required in this code. init_test is called each time a test is run,
 * and cleanup is run after every test.
 */
int init_test(void)
{
        return 0;
}

int cleanup_test(void)
{
        return 0;
}


/*
 * fireball is the code called when adding test fails: cleanup the test
 * registry and exit.
 */
void
fireball(void)
{
        CU_cleanup_registry();
        exit(CU_get_error());
}


/*
 * The main function sets up the test suite, registers the test cases,
 * runs through them, and hopefully doesn't explode.
 */
int
main(void)
{
        CU_pSuite       tsuite = NULL;
        unsigned int    fails;

        printf("\n\n[+] running tests for dirutils\n");

        if (!(CUE_SUCCESS == CU_initialize_registry())) {
                errx(EX_CONFIG, "failed to initialise test registry");
                return EXIT_FAILURE;
        }

        tsuite = CU_add_suite(TEST_SUITE, init_test, cleanup_test);
        if (NULL == tsuite)
                fireball();

        if (NULL == CU_add_test(tsuite, "path_exists", test_exists))
                fireball();

        if (NULL == CU_add_test(tsuite, "makedirs simple", test_makedirs))
                fireball();

        if (NULL == CU_add_test(tsuite, "empty dir rmdirs", test_empty_rmdirs))
                fireball();

        if (NULL == CU_add_test(tsuite, "simple rmdirs", test_rmdirs_simple))
                fireball();

        if (NULL == CU_add_test(tsuite, "full test", test_dirutils))
                fireball();

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        fails = CU_get_number_of_tests_failed();
        warnx("%u tests failed", fails);

        CU_cleanup_registry();
        return fails;
}
