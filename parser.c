#include <stdio.h>

#include "lexer.h"
#include "parser.h"

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
#define alloc(t)  ((t*)bar_alloc(P->bar, sizeof(t)))
#define curr()    (lex_curr(&P->l))
#define consume() (lex_consume(&P->l))
#define peek()    (lex_peek(&P->l))

/* parse number literals. */
static Expr *parse_num (Parser *P);

/* parse parenthesis. */
static Expr *parse_paren (Parser *P);

/* parse literals, parenthesis, etc. */
static Expr *parse_atom (Parser *P);

/* parse binary ops. */
static Expr *parse_bin_op (Parser *p, int min_prec);

/* returns `BinOpType` for token `tk`. */
static BinOpType get_bin_op (Token *tk);


void parser_init (Parser *P)
{
  lex_init(&P->l);
  P->bar = bar_new(1<<16); /* 64kB */
}

void parser_free (Parser *P)
{
  bar_free(P->bar);
  P->bar = NULL;
}

Expr *parse_expr (Parser *P)
{
  return parse_bin_op(P, 0);
}

void print_expr (Expr *node)
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

static Expr *parse_num (Parser *P)
{
  char *txt;
  size_t len;
  signed long val;
  Token *tk;
  Expr *node;
  node = alloc(Expr);

  tk = curr();
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
  consume();

  node->type = AT_NUM;
  node->val.num.tok = *tk;
  node->val.num.val = val;
  return node;
}

static Expr *parse_paren (Parser *P)
{
  Token *tk;
  Expr *node;
  consume();
  node = parse_expr(P);
  if (!node)
    return NULL;
  tk = curr();
  if (tk->tt != TK_RPAREN) {
    lex_print_token(tk, "Expected closing parenthesis.\n");
    return NULL;
  }
  consume();
  return node;
}

static Expr *parse_atom (Parser *P)
{
  Token *tk = curr();
  switch (tk->tt) {
    case TK_LNUM:    return parse_num(P);
    case TK_LPAREN:  return parse_paren(P);
    /* TODO: other literals */
    default:
      lex_print_token(tk, "Unexpected token.\n");
      return NULL;
  }
}

static Expr *parse_bin_op (Parser *P, int min_prec)
{
  Token *tk;
  Expr *left, *right, *binval;
  BinOpType op;
  int prec;

  left = parse_atom(P);
  if (!left)
    return NULL;

  for (;;) {
    binval = alloc(Expr);
    binval->type = AT_BIN_OP;

    tk = curr();
    op = get_bin_op(tk);
    binval->val.bin_op.tok = *tk;
    binval->val.bin_op.type = op;

    if (op == BOP_unknown)
      break;
    prec = get_prec(op);
    if (prec < min_prec)
      break;

    consume();
    right = parse_bin_op(P, get_asoc(op) == ASC_LEFT
                            ? min_prec + 1 : min_prec);
    if (!right)
      return NULL;

    binval->val.bin_op.left = left;
    binval->val.bin_op.right = right;
    left = binval;
  }
  return left;
}

static BinOpType get_bin_op (Token *tk)
{
  switch (tk->tt) {
    case TK_PLUS: return BOP_ADD;
    case TK_DASH: return BOP_SUB;
    default: return BOP_unknown;
  }
}
