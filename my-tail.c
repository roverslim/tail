#include <stdio.h>
#include <stdlib.h>

enum { SIZE = 1 };

void
tail (FILE* fp) {
    char *buffer;
    long offset;
 
    offset = 0;
    buffer = malloc(sizeof(char));

    while(offset > -10) {
        if (fseek(fp, offset, SEEK_END) != 0) {
            if (ferror(fp)) {
                puts("I/O error when reading");
                exit(EXIT_FAILURE);
            }
            // what to do about fseek() returning non-zero and ferror(fp) being falsey
        }

        if (fread(buffer, sizeof(char), SIZE, fp) == SIZE) {
            printf("%s", buffer);
        } else {
            if (feof(fp)) {
                printf("Reading error: unexpected end of file\n");
            } else if (ferror(fp))
                perror("Reading error");
        }
        offset--;
    }

    free(buffer);
}

int
main (void) {
    FILE* fp = fopen("sample-file.txt", "r");
    if(!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    tail(fp);
    fclose(fp);

    return EXIT_SUCCESS;
}
