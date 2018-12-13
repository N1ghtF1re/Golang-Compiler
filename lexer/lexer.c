//
// Created by alex on 08.12.18.
//

#include <stdlib.h>
#include <memory.h>
#include "lexer.h"

const int init_size = 40;
TokenNode *tokensList;

/**
 * Создание списка токенов
 * @return Указатель на голову списка
 */
TokenNode* tokensList_create() {
    tokensList = (TokenNode*) malloc(init_size * sizeof(Token));
    tokensList->next = NULL;
    return tokensList;
}

TokenNode* tokensList_push(Token token) {
    TokenNode *tmp = tokensList;
    while (tmp->next)
        tmp = tmp->next;

    TokenNode *newitem = (TokenNode*) malloc(sizeof(Token));
    newitem->next = NULL;
    newitem->token = token;

    tmp->next = newitem;


    return newitem;
}

/**
 * Добавление идентифицкатора
 * @param value Найденная строка
 * @return Адрес созданного элемента
 */
TokenNode* tokenList_add_id(char* value) {
    Token token;
    token.type = ID;

    size_t vallen = strlen(value);
    char* cpvalue = (char*) malloc(vallen * sizeof (char));
    strcpy(cpvalue, value);

    token.info.varname = cpvalue;
    token.info.isConst = 0;
    return tokensList_push(token);
}

/**
 * Добавление строкового литерала
 * TODO: ИЗВЛЕЧЕНИЕ ЗНАЧЕНИЕ ЛЮБОГО ВИДА СТРОКОВЫХ ЛИТЕРАЛОВ
 * @param value Найденная строка
 * @return Адрес созданного элемента
 */
TokenNode* tokenList_add_string_lit(char* value) {
    Token token;
    token.type = STRING_LIT;

    size_t vallen = strlen(value);
    char* cpvalue = (char*) malloc(vallen * sizeof (char));
    strcpy(cpvalue, value);

    token.info.isConst = 1;
    token.info.vartype = STRING;
    token.info.value.strvalue = cpvalue;
    return tokensList_push(token);
}

/**
 * Добавление символьного литерала
 * TODO: ИЗВЛЕЧЕНИЕ ЗНАЧЕНИЕ ЛЮБОГО ВИДА СИМВОЛЬНЫХ ЛИТЕРАЛОВ
 * @param value Найденная строка
 * @return Адрес созданного элемента
 */
TokenNode* tokenList_add_char_lit(char* value) {
    Token token;
    token.type = CHAR_LIT;

    char chr = value[1];

    token.info.isConst = 1;
    token.info.vartype = INT;
    token.info.value.intfalue = chr;
    return tokensList_push(token);
}

/**
 * Добавление ВЕЩЕСТВЕННОГО литерала
 * TODO: ИЗВЛЕЧЕНИЕ ЗНАЧЕНИЕ ЛЮБОГО ВИДА ВЕЩЕСТВЕННЫХ ЛИТЕРАЛОВ
 * @param value Найденная строка
 * @return Адрес созданного элемента
 */
TokenNode* tokenList_add_float_lit(char* value) {
    Token token;
    token.type = FLOAT_LIT;

    token.info.isConst = 1;
    token.info.vartype = FLOAT;
    token.info.value.floatvalue = strtod(value, NULL);
    return tokensList_push(token);
}


/**
 * Добавление ЦЕЛОЧИСЛЕННОГО литерала
 * TODO: ИЗВЛЕЧЕНИЕ ЗНАЧЕНИЕ ЛЮБОГО ВИДА ЦЕЛОЧИСЛЕННЫХ ЛИТЕРАЛОВ
 * @param value Найденная строка
 * @return Адрес созданного элемента
 */
TokenNode* tokenList_add_int_lit(char* value) {
    Token token;
    token.type = INT_LIT;

    token.info.isConst = 1;
    token.info.vartype = INT;
    token.info.value.intfalue = (int) strtol(value, NULL, 10);
    return tokensList_push(token);
}

/**
 * Добавление токена примитивного типа
 * @param value Найденная строка
 * @return АДрес созданного элемента
 */
TokenNode* tokenList_add_prim_type(char* value) {
    Token token;
    token.type = PRIM_TYPE;

    size_t vallen = strlen(value);
    char* cpvalue = (char*) malloc(vallen * sizeof (char));
    strcpy(cpvalue, value);


    token.info.vartype = TYPE;
    token.info.varname = cpvalue;
    token.info.value.strvalue = cpvalue;
    return tokensList_push(token);
}

TokenNode* tokenList_add_withoutvalue(TokenType tokenType) {
    Token token;
    token.type = tokenType;

    return tokensList_push(token);
}