#include "tail.h"

#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "parse_arguments.h"

#define NEWLINE '\n'

static void
print_string(FILE *fp, long fromOffset, long toOffset, FILE *stream) {
    int c;

    fseek(fp, fromOffset, SEEK_SET);

    for (long i=0L; fromOffset + i < toOffset; i++) {
        c = fgetc(fp);
        fprintf(stream, "%c", c);
    }
}

static bool
terminate(
    FILE *fp, FILE *stream, arguments_t *args,
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
            print_string(fp, 0L, fromOffset, stream);
        } else if (RELATIVE_TO_END == nDirection) {
            /*
             * Parsing from the end, we walked through the entire file not finding the number of lines requested.
             *    This results in printing out everything.
             */
            print_string(fp, 0L, maxOffset, stream);
        }

        return true;
    } else if (!rFlag && numLinesWanted == lineCount) {
        if (0 == numLinesWanted)
            print_string(fp, 0L, maxOffset, stream);
        else
            print_string(fp, fromOffset, maxOffset, stream);

        return true;
    } else if (nValueProvided && !(nValueProvided && lineCount <= numLinesWanted))
        return true;

    return false;
}

static void
set_cursor_position(FILE *fp, long iteration, long maxOffset, direction_t nDirection, bool rFlag) {
    long position;

    if (!rFlag && RELATIVE_TO_BEGINNING == nDirection)
        position = iteration;
    else
        position = maxOffset - iteration;

    fseek(fp, position, SEEK_SET);
}

static long
determine_num_lines_wanted(arguments_t *args) {
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

static void
parse_file(FILE *fp, FILE *stream, arguments_t *args) {
    bool rFlag;
    direction_t nDirection;
    int c, lineCount;
    long numLinesWanted, fromOffset, toOffset, maxOffset;

    if (NULL == fp || NULL == stream)
        return;

    fromOffset = 0L;
    fseek(fp, fromOffset, SEEK_END);
    maxOffset = toOffset = ftell(fp);

    nDirection = arguments_get_ndirection(args);
    rFlag = arguments_get_rFlag(args);

    lineCount = 0;
    numLinesWanted = determine_num_lines_wanted(args);
    for (long i=0L; i <= maxOffset; i++) {
        set_cursor_position(fp, i, maxOffset, nDirection, rFlag);
        c = fgetc(fp);

        if (NEWLINE == c) {
            lineCount++;
            fromOffset = ftell(fp);

            /*
             * Print line and continue when rFlag is set.
             *    This has the effect of printing lines in reverse.
             */
            if (rFlag) print_string(fp, fromOffset, toOffset, stream);

            /*
             * Setting the next stopgap to the beginning of the current line
             */
            toOffset = fromOffset;
        } else if (terminate(fp, stream, args, numLinesWanted, lineCount, i, fromOffset, maxOffset))
            break;
    }
}

int
tail(int argc, char **argv, FILE *stream) {
    arguments_t *args;
    char *filename;
    FILE *fp;
    int numFiles, suppressHeaders;

    args = parse_arguments(argc, argv);
    numFiles = arguments_get_numFiles(args);
    suppressHeaders = arguments_get_qFlag(args);

    for (int i = 0; (filename = arguments_get_files(args)[i]) != NULL; i++) {
        fp = fopen(filename, "r");
        if(!fp) {
            fprintf(stream, "tail: %s: No such file or directory\n", filename);
            arguments_free(args);
            return 1;
        }

        if (!suppressHeaders && numFiles > 1)
            fprintf(stream, "==> %s <==\n", filename);

        parse_file(fp, stream, args);

        if (!suppressHeaders && i + 1 < numFiles && numFiles > 1)
            fprintf(stream, "\n");

        fclose(fp);
    }

    arguments_free(args);
    return 0;
}
