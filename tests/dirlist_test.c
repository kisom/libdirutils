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
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include <dirlist.h>


void
test_dirlst_create(void)
{
        struct tq_dirlst        *lst;

        lst = dirlst_create((const char *)"foo", 3);
        CU_ASSERT(lst != NULL);
        CU_ASSERT(EXIT_SUCCESS == dirlst_destroy(&lst));
}

void
test_dirlst_one_push(void)
{
        struct tq_dirlst        *lst;
        struct dirlst           *elm;
        int                      n_elms, ret;

        lst = dirlst_create((const char *)"foo", 3);
        ret = dirlst_push(lst, (const char *)"bar", 3);
        CU_ASSERT(EXIT_SUCCESS == ret);
        n_elms = 0;
        TAILQ_FOREACH(elm, lst, dirs)
            n_elms++;

        CU_ASSERT(2 == n_elms);

        elm = dirlst_pop(lst);
        CU_ASSERT(0 == strncmp(elm->path, "bar", 3));
        free(elm);

        n_elms = 0;
        TAILQ_FOREACH(elm, lst, dirs)
            n_elms++;

        CU_ASSERT(1 == n_elms);
        CU_ASSERT(EXIT_SUCCESS == dirlst_destroy(&lst));
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

        if (NULL == CU_add_test(tsuite, "dirlst create", test_dirlst_create))
                fireball();

        if (NULL == CU_add_test(tsuite, "dirlst push/pop",
            test_dirlst_one_push))
                fireball();

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        fails = CU_get_number_of_tests_failed();
        warnx("%u tests failed", fails);

        CU_cleanup_registry();
        return fails;
}


/*
 * This is an empty test provided for reference.
 */
void
empty_test()
{
        CU_ASSERT(1 == 0);
}
