#include "helpers.h"

void
helpers_fprintf(
    FILE *fp, FILE *output,
    long fromOffset,long toOffset
) {
    int c;
    long maxOffset;

    if (NULL == fp || NULL == output) return;

    fseek(fp, 0L, SEEK_END);
    maxOffset = ftell(fp);
    if (maxOffset < fromOffset || maxOffset < toOffset) return;

    fseek(fp, fromOffset, SEEK_SET);
    for (long i=0L; fromOffset + i < toOffset; i++) {
        c = fgetc(fp);
        fprintf(output, "%c", c);
    }
}

/*
 * Position the file pointer nCharacters from the beginning or end of
 *    the file.
 */
void
helpers_set_position(FILE *fp, long nCharacters, direction_t nDirection, bool rFlag) {
    long maxOffset, position;

    if (fp == NULL) return;

    fseek(fp, 0L, SEEK_END);
    maxOffset = ftell(fp);

    if (0L >= maxOffset)
        position = 0L;
    else if (maxOffset <= nCharacters) {
        if (RELATIVE_TO_BEGINNING == nDirection)
            position = maxOffset;
        else
            position = 0L;
    } else if (!rFlag && RELATIVE_TO_BEGINNING == nDirection)
        position = nCharacters;
    else
        position = maxOffset - nCharacters;

    fseek(fp, position, SEEK_SET);
}
