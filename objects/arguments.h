#include <stdlib.h>
#include <string.h>

typedef struct arguments arguments;
arguments * init_arguments(
    const char *file,
    const char *b, const char *c, const char *n,
    int f, int F, int r
);
void free_arguments(arguments *args);
