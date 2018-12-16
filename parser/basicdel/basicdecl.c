//
// Created by alex on 16.12.18.
//

#include "basicdecl.h"

VarDeclNode *var_spec1() {
    VarDeclNode *varDeclNode = ast_create_var_decl_node();

    IdsList *idsList = ast_id_lsit_create();

    if(!id_list(idsList)) return NULL; // idList is filled by function is_list(..)


    ParamsList *paramsList = cpyIdListIntoParamList(idsList);
    if(!type()) return NULL;

    ParamsList* tmp = paramsList->next;
    while(tmp) {
        tmp->vartype = next->token;
        tmp = tmp->next;
    }

    varDeclNode->vars = paramsList;

    if(!term(ASSIGN)) return NULL;

    ExprList *exprList = ast_create_expr_list();
    if(!expr_list(exprList)) return NULL;
    varDeclNode->expressions = exprList; // Optional

    return varDeclNode;
}

VarDeclNode *var_spec2() {
    VarDeclNode *varDeclNode = ast_create_var_decl_node();

    IdsList *idsList = ast_id_lsit_create();

    if(!id_list(idsList)) return NULL; // idList is filled by function is_list(..)

    ParamsList *paramsList = cpyIdListIntoParamList(idsList);

    varDeclNode->vars = paramsList;

    if(!term(ASSIGN)) return NULL;

    ExprList *exprList = ast_create_expr_list();
    if(!expr_list(exprList)) return NULL;
    varDeclNode->expressions = exprList;
    return varDeclNode;
}

VarDeclNode *var_spec3() {
    VarDeclNode *varDeclNode = ast_create_var_decl_node();

    IdsList *idsList = ast_id_lsit_create();

    if(!id_list(idsList)) return NULL; // idList is filled by function is_list(..)


    ParamsList *paramsList = cpyIdListIntoParamList(idsList);
    if(!type()) return NULL;

    ParamsList* tmp = paramsList->next;
    while(tmp) {
        tmp->vartype = next->token;
        tmp = tmp->next;
    }

    varDeclNode->vars = paramsList;
    varDeclNode->expressions = NULL;

    return varDeclNode;
}

VarDeclNode *var_spec() {
    TokenNode *save = next;

    VarDeclNode *varDeclNode = NULL;

    if(!varDeclNode) varDeclNode = (next = save, var_spec1());
    if(!varDeclNode) varDeclNode = (next = save, var_spec2());
    if(!varDeclNode) varDeclNode = (next = save, var_spec3());

    return varDeclNode;
}

/**
 * GRAMMAR:
 * WAR_SPECS_LIST1 = VAR SPEC VAR_SPEC_LIST | EPSILON
 */
VarDeclSideList *var_specs_list() {
    TokenNode *save = next;

    VarDeclSideList *varDeclSideList = ast_var_decl_side_list_create();
    VarDeclSideList *tmp = varDeclSideList;

    bool isVarSpec = true;

    while(isVarSpec) {
        VarDeclNode *varDeclNode = var_spec();

        if(varDeclNode) {
            save = next;
            tmp->next = ast_var_decl_side_list_create();
            tmp = tmp->next;
            tmp->decl = varDeclNode;
        } else {
            next = save;
            isVarSpec = false;
        }
    }
    return varDeclSideList;
}

VarDeclSideList *var_decl1() {
    if(!(term(VAR) && term(OPEN))) return NULL; // START WITH var (

    VarDeclSideList *varDeclSideList = var_specs_list();

    if(!(term(CLOSE))) return NULL; // FINISH WITH )

    return varDeclSideList;
}

VarDeclSideList *var_decl2() {
    if(!term(VAR)) return NULL;
    VarDeclSideList *varDeclSideList = ast_var_decl_side_list_create();
    varDeclSideList->next = ast_var_decl_side_list_create();
    varDeclSideList->next->decl = var_spec();

    return (varDeclSideList->next->decl) ? varDeclSideList : NULL;
}


/**
 * Returns a list that will need to be converted to a list of declarations.
 */
VarDeclSideList *var_decl() {
    TokenNode *save = next;

    VarDeclSideList *varDecl = NULL;

    if(!(varDecl)) varDecl = (next = save, var_decl1());
    if(!(varDecl)) varDecl = (next = save, var_decl2());

    return varDecl;
}

StatementsList *declarations() {
    TokenNode *save = next;

    NodeType currNodeType = NODENONE;
    StatementsList *stmt = NULL;

    VarDeclSideList *varDeclSideList = (next = save, var_decl());
    if(varDeclSideList) { // VAR DELARATION
        stmt = ast_statements_list_create();
        StatementsList *tmpStmp = stmt;
        VarDeclSideList *removed;

        for(VarDeclSideList *tmp = varDeclSideList->next; tmp != NULL; tmp = tmp->next) {
            tmpStmp->next = ast_statements_list_create();
            tmpStmp = tmpStmp->next;
            tmpStmp->type = VAR_DECL;
            tmpStmp->statement = tmp->decl;
        }
    }

    /*
    return (next = save, var_decl())
            ;TODO: || (next = save, const_decl())
            || (next = save, type_decl());*/

    return stmt;
}