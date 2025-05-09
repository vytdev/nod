/*
 * nod -- An embedded scripting language.
 * This file is part of the 'nod' project.
 * Copyright (C) 2025  Vincent Yanzee J. Tan
 * Licensed under the MIT License.
 * See LICENSE for more info.
 * Repo:  https://github.com/vytdev/nod
 */

#ifndef NOD_COMP_H_
#define NOD_COMP_H_   1
#include "nod.h"
#include "core.h"

#define NOD_TABSTOP 4
#define nodI_cisnul(c) ((c) == '\0')
#define nodI_ciswsp(c) ((c) == ' ' || (c) == '\t')
#define nodI_cislin(c) ((c) == '\n' || (c) == '\r')
#define nodI_cisnum(c) ((c) >= '0' && (c) <= '9')
#define nodI_cislow(c) ((c) >= 'a' && (c) <= 'z')
#define nodI_cisupp(c) ((c) >= 'A' && (c) <= 'Z')
#define nodI_cisalp(c) (nodI_cislow((c)) || nodI_cisupp((c)))
#define nodI_cisanm(c) (nodI_cisalp((c)) || nodI_cisnum((c)))
#define nodI_cisidt(c) (nodI_cisalp((c)) || (c) == '_' || (c) == '$')
#define nodI_cisidn(c) (nodI_cisidt((c)) || nodI_cisnum((c)))


/* Source loc. */
typedef unsigned long sloc;


enum TokType {
  TK_NONE,
  TK_UNKNOWN,
  TK_EOF,
  TK_NEWLN,
  TK_DELIM,
  TK_IDENT,
  TK_END
};

struct Token {
  char     *text, *this_ln;
  sloc     line, col, pos, len;
  enum TokType tt;
};

struct Lexer {
  char     *src, *curr_ln;
  sloc     line, col, pos;
  int      end;
  struct Token tok, lkahead;
};


void nodI_linit (struct Lexer *l, char *src);
int nodI_lisactive (struct Lexer *l);
struct Token *nodI_lcurr (struct Lexer *l);
struct Token *nodI_lnext (struct Lexer *l);
struct Token *nodI_lpeek (struct Lexer *l);

#endif /* NOD_COMP_H_ */
