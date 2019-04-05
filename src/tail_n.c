#include "tail_n.h"

static void
display_content(FILE *fp) {
    int c;
    while ((c = fgetc(fp)) != EOF)
      putchar(c);
};

void
tail_n(FILE *fp, int num_lines_wanted) {
    int c, num_lines;
    long offset, max_offset;

    if (num_lines_wanted < 0)
      perror("Must specify 0 or more lines to tail");
    else if (num_lines_wanted == 0)
      return;
 
    num_lines = offset = 0;

    // When the input is seekable,
    //  1- go to the end
    //  2- read a char
    //  3- find out it it's a line break
    //  4- repeat steps 2 & 3 until N+1 line breaks are found
    //  5- read to standard output from the Nth+1 line break
    fseek(fp, 0L, SEEK_END);
    max_offset = ftell(fp);

    while(offset + max_offset >= 0) {
        fseek(fp, offset * sizeof(char), SEEK_END);

        c = fgetc(fp);
        if (c == '\n') {
            if (num_lines == num_lines_wanted)
                break;
            num_lines++;
        }

        offset--;
    }

    display_content(fp);
}
