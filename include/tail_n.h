#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  BLOCK_COUNT = 1,
  BLOCK_SIZE = 1 * sizeof(char)
};

void tail_n(FILE* fp, int num_lines_wanted);
