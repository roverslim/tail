#include "parse_arguments.h"

struct arguments *
parse_arguments(int argc, char **argv)
{
  char *bvalue, *cvalue, *nvalue;
  char *file;
  int fflag, Fflag, rflag;
  int c, index;
  struct arguments *args;

  file = bvalue = cvalue = nvalue = NULL;
  fflag = Fflag = rflag = 0;
  /* Supress printing of error messages by getopt */
  opterr = 0;

  while ((c = getopt (argc, argv, "b:c:fFn:r")) != -1)
    switch (c)
      {
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
      case '?':
        if (optopt == 'b' || optopt == 'c' || optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return NULL;
      default:
        abort ();
      }

  args = init_arguments(file, bvalue, cvalue, nvalue, fflag, Fflag, rflag);
  return args;
}
