#include "tail_n.h"

static void
display_content(FILE *fp) {
    int c;
    while ((c = fgetc(fp)) != EOF)
      putchar(c);
};

void
tail_n(FILE *fp, int num_lines_wanted) {
    int num_lines;
    long offset;

    if (num_lines_wanted < 0)
      perror("Must specify 0 or more lines to tail");
    else if (num_lines_wanted == 0)
      return;
 
    num_lines = 0;
    offset = -1;

    // When the input is seekable,
    //  1- go to the end
    //  2- read a block
    //  3- find out how many line breaks it contains
    //  4- repeat steps 2 & 3 until N line breaks are found
    //  5- read to standard output from the Nth line break

    while(1) {
        if (num_lines > num_lines_wanted) {
            display_content(fp);
            break;
        }

        if (fseek(fp, offset * sizeof(char), SEEK_END) != 0) {
            if (ferror(fp)) {
                perror("I/O error when reading");
                break;
            }
            // what to do about fseek() returning non-zero and ferror(fp) being falsey
        }

        int c;
        if ((c = fgetc(fp)) == EOF) {
            perror("Error reading the next character from the given input stream");
            break;
        } else {
            if (c == '\n')
                num_lines++;
        }

        offset--;
    }
}
