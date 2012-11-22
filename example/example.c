#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include <dirutils.h>


/*
 * Create the directory /tmp/foo/bar/baz/quux and remove it.
 */
int
main(void)
{
        const char mkdir_path[] = "/tmp/foo/bar/baz/quux";
        const char rmdir_path[] = "/tmp/foo";

        printf("[+] libdirutils example program\n\n");

        printf("[+] mkdir -p %s\n", mkdir_path);
        if (EXIT_FAILURE == makedirs(mkdir_path))
                err(EXIT_FAILURE, "makedirs(%s)", mkdir_path);

        printf("[+] ensuring %s exists...\n", mkdir_path);
        if (EXISTS_DIR != path_exists(mkdir_path))
                errx(EXIT_FAILURE, "%s was not created", mkdir_path);

        printf("[+] rm -r %s\n", rmdir_path);
        if (EXIT_FAILURE == rmdirs(rmdir_path))
                err(EXIT_FAILURE, "rmdirs(%s)", rmdir_path);

        printf("[+] ensuring %s was removed...\n", rmdir_path);
        if (EXISTS_NOENT != path_exists(rmdir_path))
                errx(EXIT_FAILURE, "%s was not removed", mkdir_path);
        else
                printf("[+] done.\n");
        return EXIT_SUCCESS;
}
