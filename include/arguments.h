#include "types.h"

unsigned int arguments_get_n(arguments_t *args);
bool arguments_is_nValue_provided(arguments_t *args);
int arguments_get_qFlag(arguments_t *args);
bool arguments_get_rFlag(arguments_t *args);
int arguments_get_numFiles(arguments_t *args);
char ** arguments_get_files(arguments_t *args);
direction_t arguments_get_ndirection(arguments_t *args);
arguments_t * arguments_init(
    char **files, int numFiles,
    const char *b, const char *c, const char *n,
    direction_t ndirection,
    int f, int F, bool r, int q
);
void arguments_free(arguments_t *args);
