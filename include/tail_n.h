#include "tail.h"

int tail_n(FILE *fp,
    bool nValueProvided, unsigned int nValue,
    direction_t direction, int reverse);
int tail(int argc, char **argv, FILE *stream);
