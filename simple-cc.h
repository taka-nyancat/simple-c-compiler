#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// tokenize
// type of token
typedef enum {
    TK_RESERVED, // sign
    TK_IDENT, // identifier
    TK_NUM, // int token
    TK_EOF, // token showing end of input
    TK_RETURN, // return token
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind; // kinda token
    Token *next; // next input token
    int val; // if kind is TK_NUM, val is TK_NUM's value
    char *str; // token string
    int len; // token length
    char *ty; 
};

extern Token *token;
extern char *user_input;

void error(char *format, ...);

// report error point
void error_at(char *loc, char *format, ...);

// if kinda next token is sign, this finction read next one token and return True else return False.
bool consume(char *op);

Token *consume_ident();

// if kinda next token is sign, this function read next one token  else reurn error
void expect(char *op);

// if kinda next token is intger, this read next one token and return this integer. else return error
int expect_number();

bool at_eof();

// create new token and connect cur
Token *new_token(TokenKind kind, Token *cur, char *str, int len);

bool startswith(char *p, char *q);

// tokenize input string p and return it
Token *tokenize();



// codegen
typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ, // ==
    ND_NE, //!=
    ND_LT, //<
    ND_LE, // <=
    ND_NUM, // integer
    ND_ASSIGN, // =
    ND_LVAR, // local var
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind; // type of node
    Node *lhs; // left
    Node *rhs; // right
    int val;
    int offset;
};


Node *new_node(NodeKind kind);

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);

Node *new_num(int val);

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

void gen(Node *node);
void gen_lval(Node *node);
extern Node *code[];