//
// Created by alex on 16.12.18.
//

#include "common.h"

#include "../basicdel/basicdecl.h"

/**
 * GRAMMAR:
 * STATEMENTS_LIST =
 * STATEMENT SIATEMENTS_LIST | EPSILON
 */
StatementsList *statements_list() {
    TokenNode *save = next;
    bool isDecl = true;

    StatementsList *statementsList = ast_statements_list_create();
    StatementsList *tmp = statementsList;

    while(isDecl) {
        NodeType nodeType = NODENONE;

        StatementsList *stmt = statement();
        if(stmt) {
            tmp->next = stmt;
            while(tmp->next) tmp = tmp->next;
            save = next;
        } else {
            isDecl = false;
            next = save;
        }

    }
    return statementsList;
}

StatementsList *statement() {
    TokenNode *save = next;
    StatementsList *currProduction;

    currProduction = NULL;
    if(!currProduction) currProduction = (next = save, declarations());

    return currProduction;
}

/**
 * TODO: ...
 * GRAMMAR:
 * ID_LIST TYPE
 */
ParamsList *param_decl1() {
    IdsList *idsList = ast_id_lsit_create();

    if(!id_list(idsList)) return NULL;
    if(!type()) return NULL;

    ParamsList *paramsList = cpyIdListIntoParamList(idsList);
    ParamsList *tmpParam = paramsList->next;

    while(tmpParam) {
        tmpParam->vartype = next->token;
        tmpParam = tmpParam->next;
    }

    return paramsList->next;
}

/**
 * GRAMMAR:
 * TYPE
 */
ParamsList *param_decl2() {
    ParamsList *paramsList = (ParamsList *) malloc(sizeof(ParamsList));

    if(!type()) return NULL;

    paramsList->vartype = next->token;
    Token token;
    token.type = TOKENNONE;
    paramsList->var = token;

    return paramsList;
}

ParamsList *param_decl() {
    TokenNode *save = next;

    ParamsList *paramsList = NULL;

    if(!paramsList) paramsList =  (next = save, param_decl1());
    if(!paramsList) paramsList = (next = save, param_decl2());

    return paramsList;
}



BlockNode *block() {

    BlockNode *blockNode = ast_create_block_node();
    if(!term(BLOPEN)) return NULL; // START WITH {
    blockNode->statements_list = statements_list();
    if(!term(BLCLOSE)) return NULL; // FINISH WITH }

    return blockNode;
}

/**
 * GRAMMAR:
 * ID_LIST1 = ID , ID_LIST
 */
bool id_list1(IdsList *tmpNode) {
    if(!term(ID)) return false;

    tmpNode->next = (IdsList *) malloc(sizeof(IdsList));
    tmpNode = tmpNode->next;
    tmpNode->next = NULL;
    tmpNode->token_info = next->token;

    if(!term(COMMA)) return false;

    return id_list(tmpNode);
}

/**
 * GRAMMAR:
 * ID_LIST1 = ID
 */
bool id_list2(IdsList *tmpNode) {
    if(!term(ID)) return false;
    tmpNode->next = (IdsList *) malloc(sizeof(IdsList));
    tmpNode = tmpNode->next;
    tmpNode->next = NULL;
    tmpNode->token_info = next->token;
    return true;
}

bool id_list(IdsList *tmpNode) {
    TokenNode *save = next;
    return (next = save, tmpNode->next = NULL, id_list1(tmpNode))
           || (next = save, tmpNode->next = NULL, id_list2(tmpNode));
}