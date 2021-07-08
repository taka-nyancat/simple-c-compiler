#include "simple-cc.h"

int main(int argc, char **argv)
{
    if (argc != 2){
        error("%s: invalid number of argmunts", argv[0]);
    }

    // tokenize
    user_input = argv[1];
    token = tokenize();
    Node *node = expr();

    // output asemmbly
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // generate code using tree
    gen(node);

    // load stack top 
    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}