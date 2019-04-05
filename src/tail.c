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

    FILE* fp = fopen("short-sample.txt", "r");
    if(!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    args = parse_arguments(argc, argv);
    if (tail_n(fp, arguments_get_n(args)) == 0)
        display_content(fp);

    arguments_free(args);
    fclose(fp);
    return EXIT_SUCCESS;
}
