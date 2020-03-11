#include <stdlib.h>
#include <string.h>
#include "tail_n.h"

typedef struct arguments arguments_t;

int arguments_get_n(arguments_t *args);
int arguments_get_qFlag(arguments_t *args);
int arguments_get_numFiles(arguments_t *args);
char ** arguments_get_files(arguments_t *args);
direction_t arguments_get_ndirection(arguments_t *args);
arguments_t * arguments_init(
    char **files, int numFiles,
    const char *b, const char *c, const char *n,
    direction_t ndirection,
    int f, int F, int r, int q
);
void arguments_free(arguments_t *args);
