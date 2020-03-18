#include "tail_n.h"

int
main(int argc, char **argv) {
    int status;

    status = tail(argc, argv, stdout);
    if (status == 0)
        exit(EXIT_SUCCESS);
    exit(EXIT_FAILURE);
}
