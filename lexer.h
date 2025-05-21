#ifndef NOD_LEXER_H_
#define NOD_LEXER_H_   1

#include <stddef.h>

#include "bool.h"


/* how many spaces does a single tab use. */
#define TABSTOP  (4)

typedef size_t sloc_t;

typedef struct Token Token;
typedef struct Lexer Lexer;


enum TokType
{
  TK_NONE,     /* pseudo-token */
  TK_UNKNOWN,
  TK_EOF,
  TK_LNUM,
  TK_PLUS,
  TK_DASH
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
void lex_init (Lexer *l);

/* setup a file for a lexer. */
void lex_load_file (Lexer *l, char *src, size_t len, char *name);

/* consume the current token. returns the next one */
Token *lex_consume (Lexer *l);

/* peek to the next token. */
Token *lex_peek (Lexer *l);

/* get the current token. */
Token *lex_curr (Lexer *l);

/* returns true when the lexer already has ended. */
bool_t lex_has_ended (Lexer *l);

/* prints a token to stdout. */
void lex_print_token (Token *tk, char *fmt, ...);

#endif /* NOD_LEXER_H_ */
