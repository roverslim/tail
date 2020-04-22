#include "arguments.h"
#include "types.h"

bool helpers_finish(
    FILE *fp, FILE *output,
    arguments_t *args, long numLinesWanted, int lineCount,
    long iteration, long fromOffset, long maxOffset
);
long helpers_get_num_lines_wanted(arguments_t *args);
void helpers_fprintf(
    FILE *fp, FILE *output,
    long fromOffset, long toOffset
);
void helpers_set_position(
    FILE *input,
    long iteration, long maxOffset,
    direction_t nDirection, bool rFlag
);
