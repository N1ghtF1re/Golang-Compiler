//
// Created by alex on 13.12.18.
//

#ifndef GOCOMPILER_TOKENS_H
#define GOCOMPILER_TOKENS_H

#include "../model/go_types.h"

typedef enum TokenType {
    TOKENNONE,
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
    MOD,
    ASSIGN,
    NOT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    DOT,
    COMMA,
    PACKAGE,
    ENDLINE,
    CARET,
    PLUSPLUS,
    MINUSMINUS,
    SQOPEN,
    SQCLOSE
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
 * Токен (Элемент списка)
 */
typedef struct Token {
    TokenType type;
    TokenInfo info;
} Token;


#endif //GOCOMPILER_TOKENS_H
