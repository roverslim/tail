#ifndef TAIL_N_H
#define TAIL_N_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum direction { RELATIVE_TO_BEGINNING, RELATIVE_TO_END };
typedef enum direction direction_t;

int tail_n(FILE* fp, int num_lines_wanted, direction_t direction);
#endif /* TAIL_N_H */
