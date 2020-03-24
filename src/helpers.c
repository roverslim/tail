#include "helpers.h"

void
write_buffer(FILE *fp, char **buffer) {
    int c, i = 0;

    while ((c = fgetc(fp)) != EOF) {
        sprintf((*buffer + i), "%c", c);
        i++;
    }
}
