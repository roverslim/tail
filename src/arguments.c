#include "arguments.h"
#include "tail_n.h"

enum {
  DEFAULT_N_VALUE = 10
};

struct flags {
  int f;
  int F;
  int r;
};

struct values {
  char *b;
  char *c;

  int n;
  direction_t ndirection;
};

struct arguments {
    char *file;
    struct flags flags;
    struct values values;
};

int
arguments_get_n(arguments_t *args) {
  return args->values.n;
};

direction_t
arguments_get_ndirection(arguments_t *args) {
    return args->values.ndirection;
};

arguments_t *
arguments_init(
    const char *file,
    const char *b, const char *c, const char *n,
    direction_t ndirection,
    int f, int F, int r
) {
  arguments_t *args;

  args = malloc(sizeof(arguments_t));
  if (args == NULL)
    return NULL;

  args->file = NULL;
  if (file != NULL) {
    args->file = malloc(sizeof(file) + 1);
    if (args->file == NULL) {
      arguments_free(args);
      return NULL;
    }

    args->file = strcpy(args->file, file);
  }

  args->flags.f = f;
  args->flags.F = F;
  args->flags.r = r;

  args->values.b = NULL;
  if (b != NULL) {
    args->values.b = malloc(sizeof(b) + 1);
    if (args->values.b == NULL) {
      arguments_free(args);
      return NULL;
    }

    args->values.b = strcpy(args->values.b, b);
  }

  args->values.c = NULL;
  if (c != NULL) {
    args->values.c = malloc(sizeof(c) + 1);
    if (args->values.c == NULL) {
      arguments_free(args);
      return NULL;
    }

    args->values.c = strcpy(args->values.c, c);
  }

  args->values.n = DEFAULT_N_VALUE;
  if (n != NULL)
    args->values.n = atoi(n);

  args->values.ndirection = ndirection;

  return args;
};

void
arguments_free(arguments_t *args) {
  if (args == NULL)
    return;

  if (args->file != NULL)
    free(args->file);

  if (args->values.b != NULL)
    free(args->values.b);
  if (args->values.c != NULL)
    free(args->values.c);

  free(args);
  return;
};
