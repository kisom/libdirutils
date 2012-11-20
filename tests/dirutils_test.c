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
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include <dirutils.h>

/*
 * test the use of the exists function
 */
void
test_exists(void)
{
        char            testdir[] = "testdata/testdir";
        char            testfil[] = "testdata/testfile";
        char            testnot[] = "testdata/nosuchfile";
        size_t          testdir_len, testfil_len, testnot_len;
        EXISTS_STATUS   ftype;

        testdir_len = strlen(testdir);
        testfil_len = strlen(testfil);
        testnot_len = strlen(testnot);

        ftype = path_exists(testdir, testdir_len);
        CU_ASSERT(EXISTS_DIR == ftype);

        ftype = path_exists(testfil, testfil_len);
        CU_ASSERT(EXISTS_FILE == ftype);

        ftype = path_exists(testnot, testnot_len);
        CU_ASSERT(EXISTS_NOENT == ftype);
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

        printf("[+] running tests for dirutils\n");

        if (!(CUE_SUCCESS == CU_initialize_registry())) {
                errx(EX_CONFIG, "failed to initialise test registry");
                return EXIT_FAILURE;
        }

        tsuite = CU_add_suite(TEST_SUITE, init_test, cleanup_test);
        if (NULL == tsuite)
                fireball();

        if (NULL == CU_add_test(tsuite, "path_exists", test_exists))
                fireball();

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        fails = CU_get_number_of_tests_failed();
        warnx("%u tests failed", fails);

        CU_cleanup_registry();
        return fails;
}
