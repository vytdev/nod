#ifndef NOD_PARSER_H_
#define NOD_PARSER_H_   1

#include "lexer.h"
#include "util.h"

typedef struct BinOp       BinOp;
typedef struct NumLiteral  NumLiteral;
typedef struct Expr        Expr;

typedef struct Parser      Parser;

typedef enum
{
  BOP_ADD = 0,
  BOP_SUB = 1,
  BOP_bin_cnt,
  BOP_unknown
} BinOpType;

struct BinOp
{
  Token     tok;
  BinOpType type;
  Expr     *left;
  Expr     *right;
};

struct NumLiteral
{
  Token tok;
  signed long  val;
};

typedef enum
{
  AT_BIN_OP,
  AT_NUM
} ExprType;

struct Expr
{
  ExprType    type;
  union {
    NumLiteral  num;
    BinOp       bin_op;
  } val;
};

struct Parser
{
  Lexer l;
  struct BumpArena *bar;
};

/* init a parser. */
void parser_init (Parser *P);

/* release mem used by parser. */
void parser_free (Parser *P);

/* parse number literals. */
Expr *parse_num (Parser *P);

/* parse literals, parenthesis, etc. */
Expr *parse_atom (Parser *P);

/* parse binary ops. */
Expr *parse_bin_op (Parser *p, int min_prec);

/* parse an expression. */
Expr *parse_expr (Parser *p);

/* print the expr, for debugging. */
void print_expr (Expr *node);

#endif /* NOD_PARSER_H_ */
