#include <stdlib.h>
#include <string.h>

typedef struct arguments arguments_t;
arguments_t * init_arguments(
    const char *file,
    const char *b, const char *c, const char *n,
    int f, int F, int r
);
void free_arguments(arguments_t *args);
