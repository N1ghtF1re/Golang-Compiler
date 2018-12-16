//
// Created by alex on 16.12.18.
//

#include "functions.h"

/**
 * GRAMMAR:
 * FUNC_PARAMSLIST1 = PARAM_DECL , FUNC_PARAMS
 */
bool func_paramslist1(ParamsList *tmpNode) {
    tmpNode->next = param_decl();

    if(!tmpNode->next) return false;

    while (tmpNode->next) tmpNode = tmpNode->next;
    tmpNode->next = NULL;

    if(!term(COMMA)) return false;


    return func_paramslist(tmpNode);
}

/**
 * GRAMMAR:
 * FUNC_PARAMSLIST1 = PARAM_DECL
 */
bool func_paramslist2(ParamsList *tmpNode) {
    tmpNode->next = param_decl();

    if(!tmpNode->next) return false;


    return true;
}

bool func_paramslist(ParamsList *tmpNode) {
    TokenNode *save = next;

    bool curr_production_result = false;

    if(!curr_production_result) curr_production_result = (next = save, tmpNode->next = NULL, func_paramslist1(tmpNode));
    if(!curr_production_result) curr_production_result = (next = save, tmpNode->next = NULL, func_paramslist2(tmpNode));
    if(!curr_production_result) curr_production_result = (next = save, tmpNode->next = NULL, true);
    return curr_production_result;
}

/**
 * GRAMMAR:
 * ( FUNC_PARAMSLIST )
 */
ParamsList *func_params() {
    ParamsList *paramsList = (ParamsList *) malloc(sizeof(ParamsList));
    paramsList->next = NULL;

    if(!term(OPEN)) return NULL;

    func_paramslist(paramsList);
    // if(paramsList->next == NULL) paramsList = NULL; // EMPTY ParamList

    if(!term(CLOSE)) return NULL;

    return paramsList;
}



ResultsList *func_result1() {
    ResultsList *resultsList = (ResultsList *) malloc(sizeof(ResultsList));
    resultsList->next = NULL;

    ParamsList *paramsList = func_params();

    if(!paramsList) return NULL;

    ResultsList *tmpResult = resultsList;

    /*
     * Transfer the information from the parameter list with the list of result types
     */
    for(ParamsList *tmp = paramsList->next; tmp != NULL; tmp = tmp->next) {
        tmpResult->next = (ResultsList *) malloc(sizeof(ResultsList));
        tmpResult = tmpResult->next;
        tmpResult->next = NULL;
        tmpResult->token_info = tmp->vartype;
    }

    return resultsList;
}
ResultsList *func_result2() {
    ResultsList *resultsList = (ResultsList *) malloc(sizeof(ResultsList));
    resultsList->next = (ResultsList *) malloc(sizeof(ResultsList));
    resultsList->next->next = NULL;

    if(type()) {
        resultsList->next->token_info = next->token;
        return resultsList;
    }

    return NULL;
}

ResultsList *func_result() {
    TokenNode *save = next;

    ResultsList *resultsList = NULL;

    if(!resultsList) resultsList = (next = save, func_result1());
    if(!resultsList) resultsList = (next = save, func_result2());

    return resultsList;
}


/**
 * GRAMMAR:
 * FUNC_PARAMS FUNC_RESULT
 * @return
 */
FuncSignatureSideNode *func_signature1() {
    FuncSignatureSideNode *sideNode = ast_create_signature_side_node();
    sideNode->params = func_params();
    sideNode->results = func_result();

    return (sideNode->params && sideNode->results) ? sideNode : NULL;
}


/**
 * GRAMMAR:
 * FUNC_PARAMS
 * @return
 */
FuncSignatureSideNode *func_signature2() {
    FuncSignatureSideNode *sideNode = ast_create_signature_side_node();

    sideNode->results = NULL; // In this production mussing result type delcaration

    sideNode->params = func_params();
}

FuncSignatureSideNode *func_signature() {
    TokenNode *save = next;
    FuncSignatureSideNode *sideNode = NULL;

    if(!sideNode) sideNode = (next = save, func_signature1());
    if(!sideNode) sideNode = (next = save, func_signature2());

    if(sideNode->params->next == NULL) sideNode->params = NULL; // EMPTY PARAMS LIST

    return sideNode;
}


/**
 * GRAMMAR:
 * FUNC_BODY = BLOCK
 */

BlockNode *func_body() {
    BlockNode *blockNode = block();
    if(!blockNode) return NULL;

    blockNode->type = FUNC_BODY;

    return blockNode;
}


/**
 * GRAMMAR:
 * FUNC_DELARATION1 = func id FUNC_SIGNATURE FUNC_BODY
 */
FuncDeclNode *func_declaration1() {
    FuncDeclNode *funcDeclNode = ast_create_func_decl_node();

    if(!(term(FUNC) && term(ID))) return NULL;  // START WITH TERMINALS func id
    funcDeclNode->token_info = next->token;

    FuncSignatureSideNode *signature = func_signature(); // AFTER id - params and results
    if(!signature) return NULL;


    funcDeclNode->params = signature->params;
    funcDeclNode->results = signature->results;

    funcDeclNode->body = func_body();

    if(!funcDeclNode->body) return NULL;

    return funcDeclNode;// return term(FUNC) && term(ID) && func_signature() && func_body();
}

/**
 * GRAMMAR:
 * FUNC_DELARATION2 = func id FUNC_SIGNATURE
 */
FuncDeclNode *func_declaration2() {
    FuncDeclNode *funcDeclNode = ast_create_func_decl_node();
    // In this production missing body (body = NULL)

    if(!(term(FUNC) && term(ID))) return NULL;  // START WITH TERMINALS func id
    funcDeclNode->token_info = next->token;

    FuncSignatureSideNode *signature = func_signature(); // AFTER id - params and results
    if(!signature) return NULL;


    funcDeclNode->params = signature->params;
    funcDeclNode->results = signature->results;

    return funcDeclNode;
}

FuncDeclNode *func_declaration() {
    TokenNode *save = next;

    FuncDeclNode *funcDeclNode = NULL;

    if(!funcDeclNode) funcDeclNode = (next = save, func_declaration1());
    if(!funcDeclNode) funcDeclNode = (next = save, func_declaration2());

    return funcDeclNode;
}

