#include <stdlib.h>
#include <string.h>

#define DEFAULT_N_VALUE 10

typedef struct arguments arguments_t;
int get_n(arguments_t *args);
arguments_t * init_arguments(
    const char *file,
    const char *b, const char *c, const char *n,
    int f, int F, int r
);
void free_arguments(arguments_t *args);
