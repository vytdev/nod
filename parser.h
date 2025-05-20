#ifndef NOD_PARSER_H_
#define NOD_PARSER_H_   1

#include "lexer.h"
#include "util.h"

enum BinOpType
{
  BOP_ADD = 0,
  BOP_SUB = 1,
  BOP_bin_cnt,
  BOP_unknown
};

struct AstBinOp
{
  struct Token     tok;
  enum BinOpType   type;
  struct AstNode  *left;
  struct AstNode  *right;
};

struct AstNum
{
  struct Token tok;
  signed long  val;
};

enum AstType
{
  AT_BIN_OP,
  AT_NUM
};

struct AstNode
{
  enum AstType  type;
  union {
    struct AstNum    num;
    struct AstBinOp  bin_op;
  } val;
};

/* parse number literals. */
struct AstNode *parse_num (struct Lexer *l, struct BumpArena *bar);

/* parse literals, parenthesis, etc. */
struct AstNode *parse_atom (struct Lexer *l, struct BumpArena *bar);

/* parse binary ops. */
struct AstNode *parse_bin_op (struct Lexer *l, struct BumpArena *bar,
                              int min_prec);

/* parse an expression. */
struct AstNode *parse_expr (struct Lexer *l, struct BumpArena *bar);

/* print the expr, for debugging. */
void print_expr (struct AstNode *node);

#endif /* NOD_PARSER_H_ */
