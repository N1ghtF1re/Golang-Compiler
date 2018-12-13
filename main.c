#include <stdio.h>

#include "lexer/lex.yy.c"
#define inpitfile "main.go"


void start_lexer() {
    FILE *in = fopen(inpitfile, "r");

    yyset_in(in);

    TokenNode* tokensList = tokensList_create();

    yylex();

    TokenNode *tokens = tokensList->next;

    while(tokens) {
        Token el = tokens->token;
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
        tokens = tokens->next;
    }
    fclose(in);
}

int main() {
    start_lexer();
    return 0;
}