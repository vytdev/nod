/*
 * nod -- An embedded scripting language.
 * This file is part of the 'nod' project.
 * Copyright (C) 2025  Vincent Yanzee J. Tan
 * Licensed under the MIT License.
 * See LICENSE for more info.
 * Repo:  https://github.com/vytdev/nod
 */

/*#include "nod.h"*/

#define NOD_INTERNAL
#include "comp.h"


void nodI_linit (struct Lexer *l, char *src)
{
  l->pos = 0;
  l->line = 1;
  l->col = 1;
  l->src = src;
  l->curr_ln = src;
  l->tok.tt = TK_NONE;
  l->lkahead.tt = TK_NONE;
  l->end = 0;
}


static void inc (struct Lexer *l)
{
  switch (l->src[l->pos++]) {
  case '\t':
    l->col += NOD_TABSTOP - ((l->col-1) % NOD_TABSTOP);
    break;
  case '\n':
    l->line++;
    l->curr_ln = &l->src[l->pos];
    /* fallthrough */
  case '\r':
    l->col = 1;
    break;
  default:
    l->col++;
  }
}


static char nextc (struct Lexer *l)
{
  return l->src[l->pos];
}


static struct Token tokenize (struct Lexer *l)
{
  struct Token tok;
  char c;
  if (l->end)
    return l->tok;
  tok.tt = TK_UNKNOWN;
  for (;;) {
    c = nextc(l);
    /* skip spaces (except line/cr) */
    if (nodI_ciswsp(c)) {
      while (nodI_ciswsp(c)) {
        inc(l);
        c = nextc(l);
      }
      continue;
    }
    /* skip comments */
    if (c == '#') {
      while (!nodI_cislin(c) && c != '\0') {
        inc(l);
        c = nextc(l);
      }
      continue;
    }
    tok.pos = l->pos;
    tok.line = l->line;
    tok.col = l->col;
    tok.text = &l->src[l->pos];
    tok.this_ln = l->curr_ln;
    tok.len = 1;
    /* eof */
    if (nodI_cisnul(c)) {
      inc(l);
      tok.tt = TK_EOF;
      tok.len = 1;
      l->end = 1;
      break;
    }
    /* delimeters */
    if (c == ';') {
      inc(l);
      tok.tt = TK_DELIM;
      tok.len = 1;
      break;
    }
    /* line breaks */
    if (nodI_cislin(c)) {
      inc(l);
      tok.tt = TK_NEWLN;
      tok.len = 1;
      break;
    }
    /* unknown */
    l->end = 1;
    break;
  }
  return tok;
}


int nodI_lisactive (struct Lexer *l)
{
  return !l->end;
}


struct Token *nodI_lcurr (struct Lexer *l)
{
  return &l->tok;
}


struct Token *nodI_lnext (struct Lexer *l)
{
  if (l->lkahead.tt != TK_NONE) {
    l->tok = l->lkahead;
    l->lkahead.tt = TK_NONE;
  }
  else
    l->tok = tokenize(l);
  return &l->tok;
}


struct Token *nodI_lpeek (struct Lexer *l)
{
  if (l->lkahead.tt != TK_NONE)
    l->lkahead = tokenize(l);
  return &l->lkahead;
}
