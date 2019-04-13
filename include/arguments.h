#include <stdlib.h>
#include <string.h>

typedef struct arguments arguments_t;

int arguments_get_n(arguments_t *args);
arguments_t * arguments_init(
    const char *file,
    const char *b, const char *c, const char *n,
    int f, int F, int r
);
void arguments_free(arguments_t *args);
