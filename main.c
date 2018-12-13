#include <stdio.h>

#include "lexer/lex.yy.c"


void start_lexer() {
    FILE *in = fopen("main.go", "r");
    FILE *out = fopen("test.go", "w");

    yyset_in(in);
    yyset_out(out);

    tokensList = tokensList_create();

    yylex();

    for(int i = 0; i < tokensList.size; i++) {
        Token el = tokensList.head[i];
        switch (el.type) {
            case ID:
                printf("ID. NAME: %s\n", el.info.varname);
                break;
            case STRING_LIT:
                printf("STRING_LIT. VALUE: %s\n", el.info.value.strvalue);
                break;
            case CHAR_LIT:
                printf("CHAR_LIT. VALUE: %c\n", el.info.value.intfalue);
                break;
            case FLOAT_LIT:
                printf("FLOAT_LIT. VALUE: %f\n", el.info.value.floatvalue);
                break;
            case INT_LIT:
                printf("INT_LIT. VALUE: %d\n", el.info.value.intfalue);
                break;
            case PRIM_TYPE:
                printf("PRIM_TYPE. VALUE: %s\n", el.info.varname);
                break;
            case FUNC:break;
            case VAR:break;
            case FOR:break;
            case IF:break;
            case ELSE:break;
            case OPEN:break;
            case CLOSE:break;
            case BLOPEN:break;
            case BLCLOSE:break;
            case NEQUAL:break;
            case EQUAL:break;
            case INIT:break;
            case ASSIGN:break;
            case NOT:break;
            case PLUS:break;
            case MINUS:break;
            case MUL:break;
            case DIV:break;
            case DOT:break;
            case COMMA:break;
            case PACKAGE:break;
        }
    }
    fclose(in);
    fclose(out);
}

int main() {
    start_lexer();
    return 0;
}