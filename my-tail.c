#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  BLOCK_COUNT = 1,
  BLOCK_SIZE = 1 * sizeof(char)
};

void
tail (FILE* fp, int num_lines_wanted) {
    char *buffer;
    FILE *bof;
    int num_lines;
    long offset;
 
    num_lines = offset = 0;
    buffer = malloc(BLOCK_SIZE);

    // When the input is seekable,
    //  1- go to the end
    //  2- read a block
    //  3- find out how many line breaks it contains
    //  4- repeat steps 2 & 3 until N line breaks are found
    //  5- read to standard output from the Nth line break

    while(1) {
        if (fseek(fp, offset * BLOCK_SIZE, SEEK_END) != 0) {
            if (ferror(fp)) {
                puts("I/O error when reading");
                exit(EXIT_FAILURE);
            }
            // what to do about fseek() returning non-zero and ferror(fp) being falsey
        }

        if (fread(buffer, BLOCK_SIZE, BLOCK_COUNT, fp) == BLOCK_COUNT) {
            if (strcmp("\n", buffer) == 0) {
                num_lines++;

                if (num_lines > num_lines_wanted) {
                  int c;
                  while ((c = fgetc(fp)) != EOF)
                    putchar(c);
                  break;
                }
            }
        } else {
            if (feof(fp)) {
                // No-op: we start at the end of the file
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

    tail(fp, 10);
    fclose(fp);

    return EXIT_SUCCESS;
}
