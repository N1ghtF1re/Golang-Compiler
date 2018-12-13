//
// Created by alex on 08.12.18.
//

#ifndef GOCOMPILER_LEXER_H
#define GOCOMPILER_LEXER_H

#include "../model/tokens.h"



typedef struct TokenNode {
    Token token;
    struct TokenNode* next;
} TokenNode;


// МЕТОДЫ:

TokenNode* tokensList_create();
TokenNode* tokensList_push(Token token);

TokenNode* tokenList_add_id(char* value);
TokenNode* tokenList_add_string_lit(char* value);
TokenNode* tokenList_add_char_lit(char* value);
TokenNode* tokenList_add_float_lit(char* value);
TokenNode* tokenList_add_int_lit(char* value);
TokenNode* tokenList_add_prim_type(char* value);

TokenNode* tokenList_add_withoutvalue(TokenType tokenType);






#endif //GOCOMPILER_LEXER_H
