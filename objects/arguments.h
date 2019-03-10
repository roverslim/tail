#include <stdlib.h>

struct arguments;
struct arguments * init_arguments(
    const char *file,
    const char *b, const char *c, const char *n,
    int f, int F, int r
);
void free_arguments(struct arguments *args);
