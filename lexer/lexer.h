//
// Created by alex on 08.12.18.
//

#ifndef GOCOMPILER_LEXER_H
#define GOCOMPILER_LEXER_H

#include "../go_types.h"

typedef enum TokenType {
    ID,
    STRING_LIT,
    CHAR_LIT,
    FLOAT_LIT,
    INT_LIT,
    PRIM_TYPE,
    FUNC,
    VAR,
    FOR,
    IF,
    ELSE,
    OPEN,
    CLOSE,
    BLOPEN,
    BLCLOSE,
    NEQUAL,
    EQUAL,
    INIT,
    ASSIGN,
    NOT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    DOT,
    COMMA,
    PACKAGE
} TokenType;

/**
 * Значение токена
 */
typedef union TokenValue {
    char *strvalue;
    int intfalue;
    double floatvalue;
} TokenValue;


/**
 * Информация о токене
 */
typedef struct TokenInfo {
    int isConst; // 1, если переменная - константа, иначе - 0

    char *varname; // Название переменной (Пусто, если литерал)
    VarType vartype; // Тип

    TokenValue value;
} TokenInfo;

/**
 * Токен
 */
typedef struct Token {
    TokenType type;
    TokenInfo info;
} Token;


typedef struct TokensList {
    Token* head;
    int size;
    int maxsize;
} TokensList;

TokensList tokensList_create();
Token* tokensList_push(Token token);

Token* tokenList_add_id(char* value);
Token* tokenList_add_string_lit(char* value);
Token* tokenList_add_char_lit(char* value);
Token* tokenList_add_float_lit(char* value);
Token* tokenList_add_int_lit(char* value);
Token* tokenList_add_prim_type(char* value);

Token* tokenList_add_withoutvalue(TokenType tokenType);



// МЕТОДЫ:



#endif //GOCOMPILER_LEXER_H
