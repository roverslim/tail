#include "arguments.h"

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
    char **files;
    int numFiles;
    struct flags flags;
    struct values values;
};

char **
arguments_get_files(arguments_t *args) {
    return args->files;
};

int
arguments_get_numFiles(arguments_t *args) {
    return args->numFiles;
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
    char **files, int numFiles,
    const char *b, const char *c, const char *n,
    direction_t ndirection,
    int f, int F, int r
) {
    arguments_t *args;

    args = malloc(sizeof(arguments_t));
    if (args == NULL)
        return NULL;

    args->files = files;
    args->numFiles = numFiles;

    args->flags.f = f;
    args->flags.F = F;
    args->flags.r = r;

    args->values.b = NULL;
    if (b != NULL) {
        args->values.b = malloc(strlen(b) * sizeof(char) + 1);
        if (args->values.b == NULL) {
            arguments_free(args);
            return NULL;
        }

        args->values.b = strcpy(args->values.b, b);
    }

    args->values.c = NULL;
    if (c != NULL) {
        args->values.c = malloc(strlen(c) * sizeof(char) + 1);
        if (args->values.c == NULL) {
            arguments_free(args);
            return NULL;
        }

        args->values.c = strcpy(args->values.c, c);
    }

    if (n != NULL)
        args->values.n = abs(atoi(n));
    else
        args->values.n = DEFAULT_N_VALUE;

    args->values.ndirection = ndirection;

    return args;
};

void
arguments_free(arguments_t *args) {
    if (args == NULL)
        return;

    if (args->files != NULL) {
        for (int i = 0; (args->files)[i] != NULL; i++)
            free((args->files)[i]);
        free(args->files);
    }

    if (args->values.b != NULL)
        free(args->values.b);
    if (args->values.c != NULL)
        free(args->values.c);

    free(args);
    return;
};
