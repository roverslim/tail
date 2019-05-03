#include <stdio.h>
#include <stdlib.h>

#include "parse_arguments.h"
#include "tail_n.h"

static void
display_content(FILE *fp) {
    int c;
    while ((c = fgetc(fp)) != EOF)
      putchar(c);
};

int
main(int argc, char **argv) {
    arguments_t *args;
    char *file;
    FILE *fp;

    args = parse_arguments(argc, argv);

    for (int i = 0; (file = arguments_get_files(args)[i]) != NULL; i++) {
        fp = fopen(file, "r");
        if(!fp) {
            printf("tail: %s: No such file or directory\n", file);
            exit(EXIT_FAILURE);
        }

        if (tail_n(fp, arguments_get_n(args), arguments_get_ndirection(args)) == 0)
            display_content(fp);

        fclose(fp);
    }

    arguments_free(args);
    return EXIT_SUCCESS;
}
