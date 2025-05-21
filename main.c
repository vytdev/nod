#include <stddef.h>  /* for size_t */
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "util.h"


int main (int argc, char **argv)
{
  char *data;
  size_t sz;
  Parser P;
  Expr *expr;

  if (argc < 2) {
    fprintf(stderr, "usage: %s FILE\n", argv[0]);
    return 1;
  }

  data = read_ascii_file(argv[1], &sz);
  if (!data) {
    printf("Failed to read file: %s\n", argv[1]);
    return 1;
  }

  parser_init(&P);
  lex_load_file(&P.l, data, sz, argv[1]);

  expr = parse_expr(&P);
  if (!expr)
    fatal_err("Parse failed\n");

  print_expr(expr);
  putc('\n', stdout);

  if (!lex_has_ended(&P.l))
    printf("Error: extra tokens\n");

  parser_free(&P);
  free(data);
  return 0;
}
