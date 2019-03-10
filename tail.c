#include <stdio.h>
#include <stdlib.h>

#include "parse_arguments.h"
#include "tail_n.h"

/*
 * Compile using;
 *    gcc -std=c99 -g -o my_tail objects/arguments.c tail.c parse_arguments.c tail_n.c
 */
int
main(int argc, char **argv) {
    struct arguments *args;

    args = parse_arguments(argc, argv);

    FILE* fp = fopen("sample-file.txt", "r");
    if(!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    tail_n(fp, 10);
    fclose(fp);

    return EXIT_SUCCESS;
}
