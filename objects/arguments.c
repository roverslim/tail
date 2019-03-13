#include "arguments.h"

typedef struct {
  int f;
  int F;
  int r;
} Flags;

typedef struct {
  char *b;
  char *c;
  char *n;
} Values;

struct arguments {
    char *file;
    Flags flags;
    Values values;
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
    args->file = malloc(sizeof(file) + 1);
    if (args->file == NULL) {
      free_arguments(args);
      return NULL;
    }

    args->file = strcpy(args->file, file);
  }

  (*args).flags.f = f;
  (*args).flags.F = F;
  (*args).flags.r = r;

  (*args).values.b = NULL;
  if (b != NULL) {
    (*args).values.b = malloc(sizeof(b) + 1);
    if ((*args).values.b == NULL) {
      free_arguments(args);
      return NULL;
    }

    (*args).values.b = strcpy((*args).values.b, b);
  }

  (*args).values.c = NULL;
  if (c != NULL) {
    (*args).values.c = malloc(sizeof(c) + 1);
    if ((*args).values.c == NULL) {
      free_arguments(args);
      return NULL;
    }

    (*args).values.c = strcpy((*args).values.c, c);
  }

  (*args).values.n = NULL;
  if (n != NULL) {
    (*args).values.n = malloc(sizeof(n) + 1);
    if ((*args).values.n == NULL) {
      free_arguments(args);
      return NULL;
    }

    (*args).values.n = strcpy((*args).values.n, n);
  }

  return args;
};

void
free_arguments(struct arguments *args) {
  if (args == NULL)
    return;

  if (args->file != NULL)
    free(args->file);

  if ((*args).values.b != NULL)
    free((*args).values.b);
  if ((*args).values.c != NULL)
    free((*args).values.c);
  if ((*args).values.n != NULL)
    free((*args).values.n);

  free(args);
  return;
};
