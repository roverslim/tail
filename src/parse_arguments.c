#include "parse_arguments.h"

arguments_t *
parse_arguments(int argc, char **argv)
{
    char *bvalue, *cvalue, *nvalue;
    char **files;
    char *file;
    int fflag, Fflag, rflag, qflag;
    int c;
    int numFiles;
    arguments_t *args;
    direction_t ndirection;

    files = NULL;
    file = bvalue = cvalue = nvalue = NULL;
    fflag = Fflag = rflag = qflag = 0;
    optind = 1; /* The caller can reset it to 1 to restart scanning of the same argv. (from http://man7.org/linux/man-pages/man3/getopt.3.html) */
    opterr = 0; /* Supress printing of error messages by getopt */

    while ((c = getopt(argc, argv, "b:c:fFn:rq")) != -1) {
        switch (c) {
            case 'b':
                bvalue = optarg;
                break;
            case 'c':
                cvalue = optarg;
                break;
            case 'f':
                fflag = 1;
                break;
            case 'F':
                Fflag = 1;
                break;
            case 'n':
                nvalue = optarg;
                break;
            case 'r':
                rflag = 1;
                break;
            case 'q':
                qflag = 1;
                break;
            case '?':
                if (optopt == 'b' || optopt == 'c' || optopt == 'n')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return NULL;
            default:
                abort ();
        }
    }

    numFiles = argc - optind;
    if (numFiles > 0) {
        files = malloc(numFiles * sizeof(char **) + 1);

        for (int argc_index = optind, files_index = 0; argc_index < argc; argc_index++, files_index++) {
            file = argv[argc_index];
            files[files_index] = malloc(strlen(file) * sizeof(char) + 1);
            strcpy(files[files_index], file);
        }

        files[numFiles] = NULL;
    }

    ndirection = RELATIVE_TO_END;
    if (nvalue != NULL)
        if (strncmp("+", nvalue, 1) == 0)
            ndirection = RELATIVE_TO_BEGINNING;

    args = arguments_init(
        files, numFiles,
        bvalue, cvalue, nvalue,
        ndirection,
        fflag, Fflag, rflag, qflag
    );
    return args;
}
