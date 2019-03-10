#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "objects/arguments.h"

struct arguments * parse_arguments(int argc, char **argv);
