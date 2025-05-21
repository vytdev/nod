#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "bool.h"
#include "lexer.h"

#define getc(l)  ((l)->src[(l)->pos])

/* increment the current lexical position for the current character. */
static void inc (Lexer *l);

/* process the next token. */
static Token tokenize (Lexer *l);


void lex_init (Lexer *l)
{
  l->curr.tt = l->lkah.tt = TK_NONE;
  l->len = 0;
  l->line = 1;
  l->col = 1;
  l->pos = 0;
  l->curr_ln = l->src = NULL;
  l->fname = "<unknown>";
  l->is_end = vtrue;   /* init-ed but not ready */
}

void lex_load_file (Lexer *l, char *src, size_t len, char *name)
{
  l->curr_ln = l->src = src;
  l->len = len;
  if (name)
    l->fname = name;
  l->is_end = vfalse;
}

Token *lex_consume (Lexer *l)
{
  if (l->lkah.tt != TK_NONE) {
    l->curr = l->lkah;
    l->lkah.tt = TK_NONE;
  }
  else
    l->curr = tokenize(l);
  return &l->curr;
}

Token *lex_peek (Lexer *l)
{
  if (l->lkah.tt == TK_NONE)
    l->lkah = tokenize(l);
  return &l->lkah;
}

Token *lex_curr (Lexer *l)
{
  if (l->curr.tt == TK_NONE)
    l->curr = tokenize(l);
  return &l->curr;
}

bool_t lex_has_ended (Lexer *l)
{
  return l->is_end;
}

void lex_print_token (Token *tk, char *fmt, ...)
{
  sloc_t i;

  if (fmt) {
    va_list ap;
    printf("%s: ", tk->fname);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
  }

  printf("%5lu | ", tk->line);

  /* print the token text. */
  for (i = 0; ; i++) {
    char c = tk->this_ln[i];

    switch (c) {
      case '\t': {
        /* expand tabs for proper alignment. */
        sloc_t j = TABSTOP - ((tk->col-1) % TABSTOP);
        while (0 < j)  {
          putc(' ', stdout);
          j--;
        }
        break;
      }
      case '\n': case '\0':
        /* end of file/line */
        goto stop;
      default:
        putc(c, stdout);
    }
  }
  stop:

  /* print a marker indicating where the token is. */
  putc('\n', stdout);
  printf("      | ");
  for (i = 1; i < tk->col; i++)
    putc(' ', stdout);
  for (i = 0; i < tk->len; i++)
    putc('^', stdout);
  putc('\n', stdout);
}

static void inc (Lexer *l)
{
  switch (l->src[l->pos++]) {
    case '\n':
      l->curr_ln = &l->src[l->pos];
      l->line++;
      /* fallthrough */
    case '\r':
      l->col = 1;
      break;
    case '\t':
      l->col += TABSTOP - ((l->col - 1) % TABSTOP);
      break;
    default:
      l->col++;
  }
}

static Token tokenize (Lexer *l)
{
  Token tk;
  char c;

  if (l->is_end)
    return l->curr;

  for (;;) {
    c = getc(l);

    /* skip whitespace */
    while (isspace(c)) {
      inc(l);
      c = getc(l);
    }

    tk.tt = TK_UNKNOWN;
    tk.this_ln = l->curr_ln;
    tk.line  = l->line;
    tk.col   = l->col;
    tk.pos   = l->pos;
    tk.len   = 0;
    tk.text  = &l->src[l->pos];
    tk.fname = l->fname;

    if (isdigit(c)) {
      while (isdigit(c)) {
        inc(l);
        c = getc(l);
        tk.len++;
      }
      tk.tt = TK_LNUM;
      break;
    }

    tk.len = 1;
    inc(l);
    switch (c) {
      case '\0': tk.tt = TK_EOF;     l->is_end = vtrue; break;
      case '+':  tk.tt = TK_PLUS;     break;
      case '-':  tk.tt = TK_DASH;     break;
      case '(':  tk.tt = TK_LPAREN;   break;
      case ')':  tk.tt = TK_RPAREN;   break;
      default:   tk.tt = TK_UNKNOWN; l->is_end = vtrue;
    }

    break;
  }

  return tk;
}
