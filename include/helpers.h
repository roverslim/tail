#include "arguments.h"
#include "types.h"

void helpers_fprintf(
    FILE *fp, FILE *output,
    long fromOffset, long toOffset
);
void helpers_set_position(
    FILE *fp,
    long iteration, direction_t nDirection, bool rFlag
);
