#include "helpers.h"

bool
helpers_finish(
    FILE *fp, FILE *output, arguments_t *args,
    long numLinesWanted, int lineCount,
    long iteration, long fromOffset, long maxOffset
) {
    bool nValueProvided, rFlag;
    direction_t nDirection;

    nDirection = arguments_get_ndirection(args);
    nValueProvided = arguments_is_nValue_provided(args);
    rFlag = arguments_get_rFlag(args);

    if (maxOffset == iteration) {
        if (rFlag) {
            /*
             * We've gone through the entire file, printing each line in reverse. We have yet to output the first
             *    line of the file.
             */
            helpers_fprintf(fp, output, 0L, fromOffset);
        } else if (RELATIVE_TO_END == nDirection) {
            /*
             * Parsing from the end, we walked through the entire file not finding the number of lines requested.
             *    This results in printing out everything.
             */
            helpers_fprintf(fp, output, 0L, maxOffset);
        }

        return true;
    } else if (!rFlag && numLinesWanted == lineCount) {
        if (0 == numLinesWanted)
            helpers_fprintf(fp, output, 0L, maxOffset);
        else
            helpers_fprintf(fp, output, fromOffset, maxOffset);

        return true;
    } else if (nValueProvided && !(nValueProvided && lineCount <= numLinesWanted))
        return true;

    return false;
}

long
helpers_get_num_lines_wanted(arguments_t *args) {
    bool rFlag;
    direction_t nDirection;
    long numLinesWanted;

    nDirection = arguments_get_ndirection(args);
    numLinesWanted = arguments_get_n(args);
    rFlag = arguments_get_rFlag(args);

    if (RELATIVE_TO_BEGINNING == nDirection && 0 != numLinesWanted)
        numLinesWanted--;
    else if (!rFlag && RELATIVE_TO_END == nDirection)
        numLinesWanted++;

    return numLinesWanted;
}

void
helpers_fprintf(
    FILE *fp, FILE *output,
    long fromOffset, long toOffset
) {
    int c;

    fseek(fp, fromOffset, SEEK_SET);

    for (long i=0L; fromOffset + i < toOffset; i++) {
        c = fgetc(fp);
        fprintf(output, "%c", c);
    }
}

void
helpers_set_position(FILE *fp, long iteration, long maxOffset, direction_t nDirection, bool rFlag) {
    long position;

    if (!rFlag && RELATIVE_TO_BEGINNING == nDirection)
        position = iteration;
    else
        position = maxOffset - iteration;

    fseek(fp, position, SEEK_SET);
}
