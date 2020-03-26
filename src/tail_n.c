#include "tail_n.h"

#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "parse_arguments.h"

#define NEWLINE '\n'

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
        bool nValueProvided, unsigned int nValue, direction_t direction,
        int reverseOrder) {
    int c, newlines, multiplier, origin;
    long maxOffset;

    if (fp == NULL)
        return 1;

    if (nValue == 0) {
        if (RELATIVE_TO_END == direction)
            fseek(fp, 0L, SEEK_END);
        return 0;
    } else if (nValue == 1 && RELATIVE_TO_BEGINNING == direction) {
        fseek(fp, 0L, SEEK_SET);
        return 0;
    }

    if (RELATIVE_TO_END == direction) {
        multiplier = -1;
        nValue += 1;
        origin = SEEK_END;
    } else if (RELATIVE_TO_BEGINNING == direction) {
        multiplier = 1;
        nValue -= 1;
        origin = SEEK_SET;
    } else
      return 1;
 
    newlines = 0;
    fseek(fp, 0L, SEEK_END);
    maxOffset = ftell(fp);

    for(long offset = 0; offset <= maxOffset; offset++) {
        fseek(fp, multiplier * offset * sizeof(char), origin);

        c = fgetc(fp);
        if (NEWLINE == c)
            newlines++;

        if (!reverseOrder && (newlines == nValue))
            break;

        ungetc(c, fp);
    }

    return 0;
}

int
tail(int argc, char **argv, FILE *stream) {
    arguments_t *args;
    direction_t direction;
    char *filename;
    FILE *fp;
    unsigned int nValue;
    bool nValueProvided;
    int numFiles, reverseOrder, rFlag, suppressHeaders;

    args = parse_arguments(argc, argv);
    
    direction = arguments_get_ndirection(args);
    numFiles = arguments_get_numFiles(args);
    nValue = arguments_get_n(args);
    nValueProvided = arguments_is_nValue_provided(args);
    reverseOrder = arguments_get_rFlag(args);
    rFlag = arguments_get_rFlag(args);
    suppressHeaders = arguments_get_qFlag(args);

    for (int i = 0; (filename = arguments_get_files(args)[i]) != NULL; i++) {
        fp = fopen(filename, "r");
        if(!fp) {
            fprintf(stream, "tail: %s: No such file or directory\n", filename);
            arguments_free(args);
            return -1;
        }

        if (tail_n(fp, nValueProvided, nValue, direction, reverseOrder) == 0) {

            if (!suppressHeaders && numFiles > 1)
                fprintf(stream, "==> %s <==\n", filename);
            if (reverseOrder == 0) {
                int c, i = 0;
                while ((c = fgetc(fp)) != EOF) {
                    fprintf(stream, "%c", c);
                    i++;
                }
            }
            if (!suppressHeaders && i + 1 < numFiles && numFiles > 1)
                fprintf(stream, "\n");
        }
        fclose(fp);
    }

    arguments_free(args);
    return 0;
}
