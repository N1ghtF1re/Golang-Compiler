#include <stdio.h>

#include "lexer/lex.yy.c"

void start_lexer() {
    FILE *in = fopen("main.go", "r");
    FILE *out = fopen("out", "w");
    char buffer[128];

    yyset_in(in);
    //yyset_out(out);
    yylex();
    fclose(in);
    fclose(out);
}

int main() {
    start_lexer();
    return 0;
}