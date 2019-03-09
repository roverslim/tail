#include <stdio.h>
#include <stdlib.h>
#include "tail_n.h"

int
main(void) {
    FILE* fp = fopen("sample-file.txt", "r");
    if(!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    tail_n(fp, 10);
    fclose(fp);

    return EXIT_SUCCESS;
}
