#include <stdio.h>
#include <stdlib.h>
#define NOD_INTERNAL
#include "comp.h"


static void printtok (struct Token *tok)
{
  int i;
  printf("tok: %d, pos=%lu, ln=%lu, col=%lu\n",
      tok->tt, tok->pos, tok->line, tok->col);
  printf("%6lu | ", tok->line);
  for (i = 0;;) {
    char c = tok->this_ln[i];
    switch (c) {
      case '\t': {
        int j = NOD_TABSTOP - (tok->col % NOD_TABSTOP);
        for (; j > 0; j--)
          putc(' ', stdout);
        break;
      }
      case '\n': case '\r': case '\0':
        goto stop;
      default:
        putc(tok->this_ln[i], stdout);
    }
    i++;
  }
stop:
  putc('\n', stdout);
  printf("       | ");
  for (i = 1; i < tok->col; i++)
    putc(' ', stdout);
  for (i = 0; i < tok->len; i++)
    putc('^', stdout);
  putc('\n', stdout);
}


#define BUFF_SZ 4096
static char *load_file (const char *path)
{
  FILE *fp;
  char *buf;
  size_t len;
  size_t pos;
  fp = fopen(path, "rb");
  if (!fp)
    return NULL;
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  /* +1 for NUL */
  buf = (char*)malloc(len + 1);
  if (!buf) {
    fclose(fp);
    return NULL;
  }
  for (pos = 0; pos < len;) {
    size_t rdneed, rdgot;
    rdneed = len - pos;
    rdneed = rdneed > BUFF_SZ ? BUFF_SZ : rdneed;
    rdgot = fread(buf + pos, 1, rdneed, fp);
    if (rdgot != rdneed) {
      fclose(fp);
      free(buf);
      return NULL;
    }
    pos += rdgot;
  }
  fclose(fp);
  return buf;
}


int main (int argc, char **argv)
{
  char *str;
  struct Lexer lex;
  if (argc != 2) {
    fprintf(stderr, "usage: %s FPATH\n", argv[0]);
    return -1;
  }
  str = load_file(argv[1]);
  if (!str) {
    fprintf(stderr, "Could not load file: %s\n", argv[1]);
    return -1;
  }
  nodI_linit(&lex, str);
  while (nodI_lisactive(&lex)) {
    struct Token *tok = nodI_lnext(&lex);
    printtok(tok);
  }
  return 1;
}
