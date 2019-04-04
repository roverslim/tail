#include <stdio.h>
#include <stdlib.h>

#include "parse_arguments.h"
#include "tail_n.h"

int
main(int argc, char **argv) {
    arguments_t *args;

    FILE* fp = fopen("short-sample.txt", "r");
    if(!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    args = parse_arguments(argc, argv);
    tail_n(fp, arguments_get_n(args));

    arguments_free(args);
    fclose(fp);
    return EXIT_SUCCESS;
}
