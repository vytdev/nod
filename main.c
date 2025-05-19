#include <stddef.h>  /* for size_t */
#include <stdio.h>
#include <stdlib.h>

#include "util.h"


int main (int argc, char **argv)
{
  char *data;
  size_t sz;
  if (argc < 2) {
    fprintf(stderr, "usage: %s FILE\n", argv[0]);
    return 1;
  }
  data = read_file(argv[1], &sz);
  if (!data) {
    fprintf(stderr, "%s: Failed to read file: %s\n", argv[0], argv[1]);
    return 1;
  }
  fwrite(data, 1, sz, stdout);
  free(data);
  return 0;
}
