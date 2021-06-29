#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// type of token
typedef enum {
    TK_RESERVED, // sign
    TK_NUM, // int token
    TK_EOF, // token showing end of input
} TokenKind;

typedef struct token {
    TokenKind kind; // kinda token
    struct token *next; // next input token
    int val; // if kind is TK_NUM, val is TK_NUM's value
    char *str; // token string
} Token;

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
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op){
        return false;
    }
    token = token->next;
    return true;
}

// if kinda next token is sign, this function read next one token  else reurn error
void expect(char op){
    if (token->kind != TK_RESERVED || token->str[0] != op){
        error_at(token->str, "%cではありません", op);
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
Token *new_token(TokenKind kind, Token *cur, char *str){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
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

        if (*p == '+' || *p == '-'){
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)){
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }
        error_at(p, "トークナイズできません");
    }
    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv)
{
    if (argc != 2){
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    // tokenize
    user_input = argv[1];
    token = tokenize();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("    mov rax, %d\n", expect_number());

    while (!at_eof()) {
        if (consume('+')){
            printf("    add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf("    sub rax, %d\n", expect_number());
    }

    printf("    ret\n");
    return 0;
}