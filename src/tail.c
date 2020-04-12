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

static void
tail_inner(
  FILE *fp, FILE *stream,
  bool nValueProvided, unsigned int nValue, direction_t nDirection,
  bool rFlag
) {
    int c, i, lineCount;
    long fromOffset, toOffset, maxOffset, position;

    if (NULL == fp || NULL == stream)
        return;

    fseek(fp, 0L, SEEK_END);
    maxOffset = toOffset = ftell(fp);

    if (RELATIVE_TO_BEGINNING == nDirection && 0 != nValue)
        nValue--;
    else if (!rFlag && RELATIVE_TO_END == nDirection)
        nValue++;

    i = lineCount = 0;
    for (
        long i=0L;
        i <= maxOffset &&
        (
            (nValueProvided && lineCount <= nValue) ||
            !nValueProvided
        );
        i++
    ) {
        if (!rFlag && RELATIVE_TO_BEGINNING == nDirection)
            position = i;
        else
            position = maxOffset - i;
        fseek(fp, position, SEEK_SET);
        c = fgetc(fp);

        if (maxOffset == i) {
            /*
             * Print everything once we scan the entire file without
             *  finding the number of lines requested.
             */
            if (rFlag)
                print_string(fp, 0L, fromOffset, stream);
            else if (RELATIVE_TO_END == nDirection || (RELATIVE_TO_BEGINNING == nDirection && nValue < lineCount))
                print_string(fp, 0L, maxOffset, stream);
        } else if (!rFlag && nValue == lineCount) {
            if (0 == nValue)
                print_string(fp, 0L, maxOffset, stream);
            else
                print_string(fp, fromOffset, maxOffset, stream);
            break;
        } else if (NEWLINE == c) {
            lineCount++;
            fromOffset = ftell(fp);

            /*
             * Print line and continue when rFlag is set.
             *  This has the effect of printing lines in reverse.
             */
            if (rFlag)
                print_string(fp, fromOffset, toOffset, stream);

            /*
             * Setting the next stopgap to the beginning of the current line
             */
            toOffset = fromOffset;
        }
    }
}

int
tail(int argc, char **argv, FILE *stream) {
    arguments_t *args;
    direction_t nDirection;
    char *filename;
    FILE *fp;
    unsigned int nValue;
    bool nValueProvided, reverseOrder;
    int numFiles, suppressHeaders;

    args = parse_arguments(argc, argv);
    
    nDirection = arguments_get_ndirection(args);
    numFiles = arguments_get_numFiles(args);
    nValue = arguments_get_n(args);
    nValueProvided = arguments_is_nValue_provided(args);
    reverseOrder = arguments_get_rFlag(args);
    suppressHeaders = arguments_get_qFlag(args);

    for (int i = 0; (filename = arguments_get_files(args)[i]) != NULL; i++) {
        fp = fopen(filename, "r");
        if(!fp) {
            fprintf(stream, "tail: %s: No such file or directory\n", filename);
            arguments_free(args);
            return -1;
        }

        if (!suppressHeaders && numFiles > 1)
            fprintf(stream, "==> %s <==\n", filename);

        tail_inner(fp, stream, nValueProvided, nValue, nDirection, reverseOrder);

        if (!suppressHeaders && i + 1 < numFiles && numFiles > 1)
            fprintf(stream, "\n");

        fclose(fp);
    }

    arguments_free(args);
    return 0;
}
