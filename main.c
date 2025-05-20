#include <stddef.h>  /* for size_t */
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "util.h"


int main (int argc, char **argv)
{
  char *data;
  size_t sz;
  struct Lexer l;
  if (argc < 2) {
    fprintf(stderr, "usage: %s FILE\n", argv[0]);
    return 1;
  }
  data = read_file(argv[1], &sz);
  if (!data) {
    fprintf(stderr, "%s: Failed to read file: %s\n", argv[0], argv[1]);
    return 1;
  }

  lex_init(&l);
  lex_load_file(&l, data, sz, argv[1]);
  while (!lex_has_ended(&l)) {
    struct Token *tk;
    tk = lex_next(&l);
    lex_print_token(tk, "token print: type=%d\n", tk->tt);
  }

  free(data);
  return 0;
}
