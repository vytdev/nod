#ifndef NOD_LEXER_H_
#define NOD_LEXER_H_   1

#include <stddef.h>

#include "bool.h"


/* how many spaces does a single tab use. */
#define TABSTOP  (4)

typedef size_t sloc_t;


enum TokType
{
  TK_NONE,     /* pseudo-token */
  TK_UNKNOWN,
  TK_EOF,
};

struct Token
{
  enum TokType tt;
  size_t     len;
  sloc_t     line, col, pos;
  char      *this_ln, *text, *fname;
};

struct Lexer
{
  struct Token  curr, lkah;
  size_t     len;
  sloc_t     line, col, pos;
  char      *curr_ln, *src, *fname;
  bool_t     is_end;
};

/* setup the lexer. */
void lex_init (struct Lexer *l);

/* setup a file for a lexer. */
void lex_load_file (struct Lexer *l, char *src, size_t len, char *name);

/* get the next token. consume the current one. */
struct Token *lex_next (struct Lexer *l);

/* peek to the next token. */
struct Token *lex_peek (struct Lexer *l);

/* get the current token. */
struct Token *lex_curr (struct Lexer *l);

/* returns true when the lexer already has ended. */
bool_t lex_has_ended (struct Lexer *l);

/* prints a token to stdout. */
void lex_print_token (struct Token *tk, char *fmt, ...);

#endif /* NOD_LEXER_H_ */
