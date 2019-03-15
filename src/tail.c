#include <stdio.h>
#include <stdlib.h>

#include "parse_arguments.h"
#include "tail_n.h"

int
main(int argc, char **argv) {
    arguments_t *args;

    args = parse_arguments(argc, argv);
    free_arguments(args);

    FILE* fp = fopen("sample-file.txt", "r");
    if(!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    tail_n(fp, 10);
    fclose(fp);

    return EXIT_SUCCESS;
}
