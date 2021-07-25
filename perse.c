#include "simple-cc.h"

Token *token;

// user input
char *user_input;

// fucntion return error signal
void error(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// report error point
void error_at(char *loc, char *format, ...){
    va_list ap;
    va_start(ap, format);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// if kinda next token is sign, this finction read next one token and return True else return False.
bool consume(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) !=  token->len || memcmp(token->str, op, token->len)){
        return false;
    }
    token = token->next;
    return true;
}

Token *consume_ident(){
    if (token->kind != TK_IDENT) {
        return NULL;
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

// if kinda next token is sign, this function read next one token  else reurn error
void expect(char *op){
    if (token->kind != TK_RESERVED || strlen(op) !=  token->len || memcmp(token->str, op, token->len)){
        error_at(token->str, "expected \"%s\"", op);
    }
    token = token->next;
}
// if kinda next token is intger, this read next one token and return this integer. else return error
int expect_number(){
    if (token->kind != TK_NUM){
        error_at(token->str, "数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof(){
    return token->kind == TK_EOF;
}

// create new token and connect cur
Token *new_token(TokenKind kind, Token *cur, char *str, int len){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

bool startswith(char *p, char *q){
    return memcmp(p, q, strlen(q)) == 0;
}

// tokenize input string p and return it
Token *tokenize(){
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // skip space
        if (isspace(*p)){
            p++;
            continue;
        }

        if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>;=", *p)){
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (isdigit(*p)){
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            cur = new_token(TK_IDENT, cur, p++, 0);
            cur->len = 1;
            continue;
        }

        error_at(p, "invalid token");
    }
    new_token(TK_EOF, cur, p, 0);
    return head.next;
}