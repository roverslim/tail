#include "tail_n.h"

#define NEWLINE '\n'

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
tail_n(FILE *fp, int num_lines_wanted, direction_t direction) {
    int newlines, multiplier, origin;
    long max_offset;

    if (num_lines_wanted < 0)
      return 1;
    else if (num_lines_wanted == 0)
      return 0;

    if (RELATIVE_TO_END == direction) {
        multiplier = -1;
        num_lines_wanted += 1;
        origin = SEEK_END;
    } else if (RELATIVE_TO_BEGINNING == direction) {
        multiplier = 1;
        num_lines_wanted -= 1;
        origin = SEEK_SET;
    } else
      return 1;
 
    newlines = 0;
    fseek(fp, 0L, SEEK_END);
    max_offset = ftell(fp);

    for(long offset = 0; offset <= max_offset; offset++) {
        fseek(fp, multiplier * offset * sizeof(char), origin);

        if (NEWLINE == fgetc(fp))
            newlines++;

        if (newlines == num_lines_wanted)
            break;
    }

    return 0;
}
