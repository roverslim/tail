#include "arguments.h"

struct arguments {
    char *file;
    union {
        struct { int f, F, r; } flags;
        struct { char *b, *c, *n; } values;
    };
};

struct arguments *
init_arguments(
    const char *file,
    const char *b, const char *c, const char *n,
    int f, int F, int r
) {
  struct arguments *args;

  args = malloc(sizeof(struct arguments));
  if (args == NULL)
    return NULL;

  args->file = NULL;
  if (file != NULL) {
    args->file = malloc(sizeof(file));
    if (args->file == NULL) return NULL;
  }

  args->flags.f = f;
  args->flags.F = F;
  args->flags.r = r;

  args->values.b = NULL;
  if (b != NULL) {
    args->values.b = malloc(sizeof(b));
    if (args->values.b == NULL) return NULL;
  }

  args->values.c = NULL;
  if (c != NULL) {
    args->values.c = malloc(sizeof(c));
    if (args->values.c == NULL) return NULL;
  }

  args->values.n = NULL;
  if (n != NULL) {
    args->values.n = malloc(sizeof(n));
    if (args->values.n == NULL) return NULL;
  }

  return args;
};

void
free_arguments(struct arguments *args) {
  if (args == NULL)
    return;

  if (args->file != NULL)
    free(args->file);

  if (args->values.b != NULL)
    free(args->values.b);
  if (args->values.c != NULL)
    free(args->values.c);
  if (args->values.n != NULL)
    free(args->values.n);

  return;
};
