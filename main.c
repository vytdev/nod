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
  struct Lexer l;
  struct BumpArena *bar;
  struct AstNode *expr;

  if (argc < 2) {
    fprintf(stderr, "usage: %s FILE\n", argv[0]);
    return 1;
  }

  data = read_file(argv[1], &sz);
  if (!data) {
    printf("Failed to read file: %s\n", argv[1]);
    return 1;
  }

  lex_init(&l);
  lex_load_file(&l, data, sz, argv[1]);

  bar = bar_new(4096);
  if (!bar) {
    printf("out of memory\n");
    return 1;
  }

  expr = parse_expr(&l, bar);
  if (!expr) {
    printf("Parse failed\n");
    bar_free(bar);
    return 1;
  }
  print_expr(expr);
  putc('\n', stdout);

  if (!lex_has_ended(&l))
    printf("Error: extra tokens\n");

  bar_free(bar);
  free(data);
  return 0;
}
