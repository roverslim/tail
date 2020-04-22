#include "tail.h"

#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "helpers.h"
#include "parse_arguments.h"

#define NEWLINE '\n'

static void
parse(FILE *fp, FILE *output, arguments_t *args) {
    bool rFlag;
    direction_t nDirection;
    int c, lineCount;
    long numLinesWanted, fromOffset, toOffset, maxOffset;

    if (NULL == fp || NULL == output)
        return;

    fromOffset = 0L;
    fseek(fp, fromOffset, SEEK_END);
    maxOffset = toOffset = ftell(fp);

    nDirection = arguments_get_ndirection(args);
    rFlag = arguments_get_rFlag(args);

    lineCount = 0;
    numLinesWanted = helpers_get_num_lines_wanted(args);
    for (long i=0L; i <= maxOffset; i++) {
        helpers_set_position(fp, i, maxOffset, nDirection, rFlag);
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
        } else if (helpers_finish(fp, output, args, numLinesWanted, lineCount, i, fromOffset, maxOffset))
            break;
    }
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
