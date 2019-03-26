#include <stdio.h>
#include <stdlib.h>

#include "parse_arguments.h"
#include "tail_n.h"

int
main(int argc, char **argv) {
    arguments_t *args;

    FILE* fp = fopen("sample-file.txt", "r");
    if(!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    args = parse_arguments(argc, argv);
    tail_n(fp, get_n(args));

    free_arguments(args);
    fclose(fp);
    return EXIT_SUCCESS;
}
