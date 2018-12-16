//
// Created by alex on 13.12.18.
//

#ifndef GOCOMPILER_PARSER_H
#define GOCOMPILER_PARSER_H

#define bool int
#define true 1
#define false 0

#include "../lexer/lexer.h"
#include "../model/ast.h"

TokenNode* next; // Pointer to the next token

void parser_start(TokenNode *tokens);
bool term (TokenType token);
bool type();
bool literal();
#endif //GOCOMPILER_PARSER_H
