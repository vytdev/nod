#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "util.h"

struct BinOpInfo
{
  int prec;
  enum { ASC_LEFT, ASC_RIGHT } asc; /* associativity */
};

static struct BinOpInfo bin_info[BOP_bin_cnt] = {
  { 0, ASC_LEFT },  /* BOP_ADD */
  { 0, ASC_LEFT },  /* BOP_SUB */
};

#define get_prec(op) (bin_info[(op)].prec)
#define get_asoc(op) (bin_info[(op)].asc)

/* returns `enum BinOpType` for token `tk`. */
static enum BinOpType get_bin_op (struct Token *tk);


struct AstNode *parse_num (struct Lexer *l, struct BumpArena *bar)
{
  char *txt;
  size_t len;
  signed long val;
  struct Token *tk;
  struct AstNode *node;
  node = (struct AstNode*)bar_alloc(bar, sizeof(struct AstNode));
  if (!node)
    return NULL;
  tk = lex_curr(l);
  if (tk->tt != TK_LNUM) {
    lex_print_token(tk, "Expected number literal.\n");
    return NULL;
  }
  len = tk->len;
  txt = tk->text;
  val = 0;
  while (len > 0) {
    val *= 10;
    val += *txt - '0';
    txt++;
    len--;
  }
  lex_next(l);
  node->type = AT_NUM;
  node->val.num.tok = *tk;
  node->val.num.val = val;
  return node;
}

struct AstNode *parse_atom (struct Lexer *l, struct BumpArena *bar)
{
  /* TODO: add parenthesis, and other literals */
  return parse_num(l, bar);
}

struct AstNode *parse_bin_op (struct Lexer *l, struct BumpArena *bar,
                              int min_prec)
{
  struct Token *tk;
  struct AstNode *left, *right, *binval;
  enum BinOpType op;
  int prec;
  left = parse_atom(l, bar);
  if (!left)
    return NULL;
  for (;;) {
    binval = (struct AstNode*)bar_alloc(bar, sizeof(struct AstNode));
    if (!binval)
      return NULL;
    binval->type = AT_BIN_OP;

    tk = lex_curr(l);
    op = get_bin_op(tk);
    binval->val.bin_op.tok = *tk;
    binval->val.bin_op.type = op;

    if (op == BOP_unknown)
      break;
    prec = get_prec(op);
    if (prec < min_prec)
      break;

    lex_next(l);
    right = parse_bin_op(l, bar, get_asoc(op) == ASC_LEFT
                    ? min_prec + 1 : min_prec);
    if (!right)
      return NULL;

    binval->val.bin_op.left = left;
    binval->val.bin_op.right = right;
    left = binval;
  }
  return left;
}

struct AstNode *parse_expr (struct Lexer *l, struct BumpArena *bar)
{
  return parse_bin_op(l, bar, 0);
}

void print_expr (struct AstNode *node)
{
  if (!node) {
    printf("NULL");
    return;
  }
  switch (node->type) {
    case AT_BIN_OP:
      putc('(', stdout);
      print_expr(node->val.bin_op.left);
      putc(' ', stdout);
      fwrite(node->val.bin_op.tok.text, 1, node->val.bin_op.tok.len, stdout);
      putc(' ', stdout);
      print_expr(node->val.bin_op.right);
      putc(')', stdout);
      break;
    case AT_NUM:
      printf("%li", node->val.num.val);
      break;
    default:
      printf("<\?\?>");
  }
}

static enum BinOpType get_bin_op (struct Token *tk)
{
  switch (tk->tt) {
    case TK_PLUS: return BOP_ADD;
    case TK_DASH: return BOP_SUB;
    default: return BOP_unknown;
  }
}
