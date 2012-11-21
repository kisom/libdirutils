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


/*
 * helper that, given a list and the expected key, determines whether
 * the the popped path matches. Return values use if semantics: success
 * is 1, failure is 0. It is up to the caller to ensure that keylen is
 * in the acceptable range (FILENAME_MAX) for tests that are to be
 * successful.
 */
int
test_pop_helper(struct tq_dirlst *lst, const char *key, size_t keylen)
{
        struct dirlst   *elm;
        int              match;

        if (keylen > FILENAME_MAX)
                return 0;

        elm = dirlst_pop(lst);
        if (NULL == elm)
                return 0;
        match = strncmp(elm->path, key, keylen);
        free(elm);
        return 0 == match;
}


void
test_dirlst_create(void)
{
        struct tq_dirlst        *lst;

        lst = dirlst_create((const char *)"foo", 3);
        CU_ASSERT(lst != NULL);
        CU_ASSERT(EXIT_SUCCESS == dirlst_destroy(&lst));
}

void
test_dirlst_single(void)
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

        CU_ASSERT(test_pop_helper(lst, "bar", 3));
        n_elms = 0;
        TAILQ_FOREACH(elm, lst, dirs)
            n_elms++;

        CU_ASSERT(1 == n_elms);
        CU_ASSERT(EXIT_SUCCESS == dirlst_destroy(&lst));
}


/*
 * ensure an invalid path_len aborts the push
 */
void
test_dirlst_bad(void)
{
        struct tq_dirlst        *lst;

        lst = dirlst_create((const char *)"foo", 3);
        CU_ASSERT(EXIT_SUCCESS == dirlst_push(lst, "bar", 3));
        CU_ASSERT(EXIT_FAILURE == dirlst_push(lst, "baz", FILENAME_MAX * 2));
        CU_ASSERT(test_pop_helper(lst, "bar", 3));
        CU_ASSERT(EXIT_SUCCESS == dirlst_destroy(&lst));
}


void
test_dirlst_multi(void)
{
        char                     testkeys[][4] = {"bar", "baz", "quux", ""};
        int                      testkeylen[4];
        struct tq_dirlst        *lst;
        struct dirlst           *elm;
        int                      i, n_elms, ret;

        lst = dirlst_create((const char *)"foo", 3);
        for(i = 0; i < 3; ++i) {
                testkeylen[i] = strlen(testkeys[i]);
                ret = dirlst_push(lst, testkeys[i], testkeylen[i]);
                CU_ASSERT(EXIT_SUCCESS == ret);
        }

        n_elms = 0;
        TAILQ_FOREACH(elm, lst, dirs)
                n_elms++;
        CU_ASSERT(4 == n_elms);

        for (i = 2; i >= 0; i--)
                CU_ASSERT(test_pop_helper(lst, testkeys[i], testkeylen[i]));
        CU_ASSERT(test_pop_helper(lst, "foo", 3));
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

        if (NULL == CU_add_test(tsuite, "single push/pop", test_dirlst_single))
                fireball();

        if (NULL == CU_add_test(tsuite, "multi push/pop", test_dirlst_multi))
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
