#include "tail_n.h"

#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "parse_arguments.h"

#define NEWLINE '\n'

void
print_string(FILE *fp, long fromOffset, long toOffset, FILE *stream) {
    int c;

    fseek(fp, fromOffset, SEEK_SET);

    for (long i=0L; fromOffset + i < toOffset; i++) {
        c = fgetc(fp);
        fprintf(stream, "%c", c);
    }
}

void
tail_r(FILE *fp, FILE *stream, bool nValueProvided, unsigned int nValue, direction_t nDirection) {
    int c, i, lineCount;
    long fromOffset, toOffset, maxOffset;

    fseek(fp, 0L, SEEK_END);
    maxOffset = toOffset = ftell(fp);

    if (nDirection == RELATIVE_TO_BEGINNING)
        nValue--;

    i = lineCount = 0;
    for (int i=0; i <= maxOffset && ((nValueProvided && lineCount <= nValue) || !nValueProvided); i++) {
        fseek(fp, maxOffset - i, SEEK_SET);
        c = fgetc(fp);

        if (maxOffset == i) {
            print_string(fp, 0L, toOffset, stream);
        } else if (NEWLINE == c) {
            lineCount++;

            fromOffset = ftell(fp);
            print_string(fp, fromOffset, toOffset, stream);

            // Set the last offset to the edge of what was just printed
            toOffset = fromOffset;
        }
    }
}

void
set_pointer(FILE *fp, long  maxOffset, int multiplier, unsigned int nValue, int origin) {
    int c, newlines = 0;

    for(long offset = 0; offset <= maxOffset; offset++) {
        long o = multiplier * offset * sizeof(char);
        fseek(fp, o, origin);

        c = fgetc(fp);

        if (NEWLINE == c)
            newlines++;

        if (newlines == nValue)
            break;

        ungetc(c, fp);
    }
}

/*
    tail_n

    Sets the file position indicator for the file stream stream to the Nth-last line break.

    When the input is seekable:
        1- go to the end of the file stream (or start from the beginning),
        2- read the preceeding char (if there is one),
        3- find out it it's a line break,
        4- repeat steps 2 & 3 until N line breaks are found

    **Return value**
    `0` upon success. Non-zero value otherwise.
*/
int
tail_n(FILE *fp,
        bool nValueProvided, unsigned int nValue, direction_t nDirection) {
    int origin, multiplier;
    long maxOffset;

    if (fp == NULL)
        return 1;

    if (nValue == 0) {
        if (RELATIVE_TO_END == nDirection)
            fseek(fp, 0L, SEEK_END);
        return 0;
    } else if (nValue == 1 && RELATIVE_TO_BEGINNING == nDirection) {
        fseek(fp, 0L, SEEK_SET);
        return 0;
    }

    fseek(fp, 0L, SEEK_END);
    maxOffset = ftell(fp);

    if (RELATIVE_TO_END == nDirection) {
        multiplier = -1;
        nValue += 1;
        origin = SEEK_END;
    } else if (RELATIVE_TO_BEGINNING == nDirection) {
        multiplier = 1;
        nValue -= 1;
        origin = SEEK_SET;
    } else
      return 1;
 
    set_pointer(fp, maxOffset, multiplier, nValue, origin);

    return 0;
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

        if (reverseOrder && nValue != 0) { // TODO: remove nValue != 0
            tail_r(fp, stream, nValueProvided, nValue, nDirection);
        } else if (!reverseOrder) {
            tail_n(fp, nValueProvided, nValue, nDirection);

            if (!suppressHeaders && numFiles > 1)
                fprintf(stream, "==> %s <==\n", filename);

            int c, i = 0;
            while ((c = fgetc(fp)) != EOF) {
                fprintf(stream, "%c", c);
                i++;
            }

            if (!suppressHeaders && i + 1 < numFiles && numFiles > 1)
                fprintf(stream, "\n");
        }
        fclose(fp);
    }

    arguments_free(args);
    return 0;
}
