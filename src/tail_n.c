#include "tail_n.h"

/*
    tail_n

    Sets the file position indicator for the file stream stream to the Nth-last line break.

    When the input is seekable:
        1- go to the end,
        2- read the preceeding char (if there is one),
        3- find out it it's a line break,
        4- repeat steps 2 & 3 until N line breaks are found

    **Return value**
    `0` upon success. Non-zero value otherwise.
*/
int
tail_n(FILE *fp, int num_lines_wanted) {
    int c, num_lines;
    long offset, max_offset;

    if (num_lines_wanted < 0) {
      perror("Must specify 0 or more lines to tail");
      return 1;
    } else if (num_lines_wanted == 0)
      return 0;
 
    num_lines = offset = 0;
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

    return 0;
}
