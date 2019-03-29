#include "tail_n.h"

void
tail_n(FILE* fp, int num_lines_wanted) {
    char *buffer;
    int num_lines;
    long offset;
    long int beginning_pos;

    if (num_lines_wanted < 0)
      perror("Must specify 0 or more lines to tail");
 
    num_lines = 0;
    offset = -1;
    buffer = malloc(BLOCK_SIZE);

    // When the input is seekable,
    //  1- go to the end
    //  2- read a block
    //  3- find out how many line breaks it contains
    //  4- repeat steps 2 & 3 until N line breaks are found
    //  5- read to standard output from the Nth line break
    fseek(fp, offset * BLOCK_SIZE, SEEK_SET);
    beginning_pos = ftell(fp);

    while(1) {
        if (fseek(fp, offset * BLOCK_SIZE, SEEK_END) != 0) {
            if (ferror(fp)) {
                puts("I/O error when reading");
                exit(EXIT_FAILURE);
            }
            // what to do about fseek() returning non-zero and ferror(fp) being falsey
        }

        if (num_lines > num_lines_wanted || beginning_pos == ftell(fp)) {
          int c;
          while ((c = fgetc(fp)) != EOF)
            putchar(c);
          break;
        }

        if (fread(buffer, BLOCK_SIZE, BLOCK_COUNT, fp) == BLOCK_COUNT) {
            if (strcmp("\n", buffer) == 0) {
                num_lines++;
            }
        } else {
            if (feof(fp)) {
                puts("Arrived at end of file");
                break;
            } else if (ferror(fp))
                perror("Reading error");
        }
        offset--;
    }

    free(buffer);
}
