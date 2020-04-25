#include "tail.h"

#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "helpers.h"
#include "parse_arguments.h"

#define NEWLINE '\n'

static bool
continue_parsing(arguments_t *args, long iteration, long lineCount, long maxOffset, long numLinesWanted) {
    bool nValueProvided, rFlag;

    nValueProvided = arguments_is_nValue_provided(args);
    rFlag = arguments_get_rFlag(args);

    if (maxOffset <= iteration)
        return false;
    else if (!rFlag && numLinesWanted == lineCount)
        return false;
    else if (nValueProvided && !(nValueProvided && lineCount <= numLinesWanted))
        return false;

    return true;
}

static long
num_lines_wanted(arguments_t *args) {
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
print_output(
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
    } else if (!rFlag && numLinesWanted == lineCount) {
        if (0 == numLinesWanted)
            helpers_fprintf(fp, output, 0L, maxOffset);
        else
            helpers_fprintf(fp, output, fromOffset, maxOffset);
    }
}

static void
parse(FILE *fp, FILE *output, arguments_t *args) {
    bool rFlag;
    direction_t nDirection;
    int c, lineCount;
    long iterations, numLinesWanted, fromOffset, toOffset, maxOffset;

    if (NULL == fp || NULL == output)
        return;

    fromOffset = 0L;
    fseek(fp, fromOffset, SEEK_END);
    maxOffset = toOffset = ftell(fp);

    nDirection = arguments_get_ndirection(args);
    rFlag = arguments_get_rFlag(args);

    iterations = 0L;
    lineCount = 0;
    numLinesWanted = num_lines_wanted(args);
    for (
        ;
        continue_parsing(args, iterations, lineCount, maxOffset, numLinesWanted);
        iterations++
    ) {
        helpers_set_position(fp, iterations, nDirection, rFlag);
        c = fgetc(fp);

        if (NEWLINE == c) {
            lineCount++;
            fromOffset = ftell(fp);

            /*
             * Print line and continue when rFlag is set.
             *    This has the effect of printing lines in reverse.
             */
            if (rFlag) helpers_fprintf(fp, output, fromOffset, toOffset);

            /*
             * Setting the next stopgap to the beginning of the current line
             */
            toOffset = fromOffset;
        }
    }

    print_output(fp, output, args, numLinesWanted, lineCount, iterations, fromOffset, maxOffset);
}

int
tail(int argc, char **argv, FILE *output) {
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
            fprintf(output, "tail: %s: No such file or directory\n", filename);
            arguments_free(args);
            return 1;
        }

        if (!suppressHeaders && numFiles > 1)
            fprintf(output, "==> %s <==\n", filename);

        parse(fp, output, args);

        if (!suppressHeaders && i + 1 < numFiles && numFiles > 1)
            fprintf(output, "\n");

        fclose(fp);
    }

    arguments_free(args);
    return 0;
}
