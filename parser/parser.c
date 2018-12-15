//
// Created by alex on 13.12.18.
//
#include <stdio.h>
#include "parser.h"

TokenNode* next; // Pointer to the next token

/**
 * Checks the token to be transferred and the next token from the token stream for equivalence
 * @param token Token struct
 * @return true if tokens are equivalent
 */
bool term (TokenType token) {
    if(next ->next == NULL) return false;
    return (next = next->next)->token.type == token;
}

bool id_list(IdsList *tmpNode); // PREDECLARATION
bool func_paramslist();
DeclSectionNode *top_level_declarations();
bool statements_list();
bool var_specs_list();
bool expr_list();
bool var_decl();
bool expression();
bool postfix_expr();
bool array_or_cut();
bool func_call();

/**
 * GRAMMAR:
 * PACKAGE DIFINITION = package identificator
 *
 * TODO: передалать узел с типом на нетерминальный
 */
PackageNode *package_difinition() {
    if(!term(PACKAGE)) return NULL;
    if(!term(ID)) return NULL;
    PackageNode *packageNode = (PackageNode *) malloc(sizeof(PackageNode));
    packageNode->id = next->token;
    return packageNode;
}


bool type() {
    TokenNode *save = next;
    return (next = save, term(PRIM_TYPE))
           || (next = save, term(ID));
}

bool postfix_expr1() {
    return term(ID) && term(PLUSPLUS);
}
bool postfix_expr2() {
    return term(ID) && term(MINUSMINUS);
}

bool array_or_cur1() {
    return term(SQOPEN) && expression() && term(SQCLOSE) && array_or_cut();
}

bool array_or_cut2() {
    return term(SQOPEN) && expression() && term(SQCLOSE);
}
/**
 * GRAMMAR:
 * ARRAY_OR_CUR = ( EXPRESSION ) ARRAY_OR_CUR
 *              | ( EXPRESSION )
 */
bool array_or_cut() {
    TokenNode *save = next;
    return (next = save, array_or_cur1())
           || (next = save, array_or_cut2());
}

bool postfix_expr3() {
    return term(ID) && array_or_cut();
}

bool func_call1() {
    return false; //TODO: term(OPEN) && id_list() && term(CLOSE) && func_call();
}
bool func_call12() {
    return false; // TODO: term(OPEN) && id_list() && term(CLOSE);
}

/**
 * GRAMMAR:
 * FUNC_CALL = ( ID_LIST ) FUNC_CALL
 *           | ( ID_LIST )
 */
bool func_call() {
    TokenNode *save = next;
    return (next = save, func_call1())
           || (next = save, func_call12());
}


bool postfix_expr4() {
    return term(ID) && func_call();
}
bool postfix_expr5() {
    return term(ID);
}

/**
 * GRAMMAR:
 * POSTFIX_EXPRESSION = ID ++
 *                  |   ID --
 *                  |   ID FUNC_CALL
 *                  |   ID ARRAY_OR_CUT
 *                  |   ID
 */
bool postfix_expr() {
    TokenNode *save = next;
    return (next = save, postfix_expr1())
           || (next = save, postfix_expr2())
           || (next = save, postfix_expr3())
           || (next = save, postfix_expr4())
           || (next = save, postfix_expr5());
}


bool mul_expr() {

}


/**
 * TODO: _
 */
bool expression() {
    return true;
}


/**
 * GRAMMAR:
 * VAR_SPECS_LIST1 = VAR_SPEC VAR_SPEC_LIST
 */
bool expr_list1() {
    return expression() && term(COMMA) && expr_list();
}

bool expr_list2() {
    return expression();
}

/**
 * GRAMMAR:
 * EXPR_LIST = EXPR_LIST | EPSILON
 */
bool expr_list() {
    TokenNode *save = next;
    return (next = save, expr_list1())
           || (next = save, expr_list2());
}

bool var_spec1() {
    return false; //TODO: id_list() && type() && term(ASSIGN) && expr_list();
}

bool var_spec2() {
    return false; // TODO: id_list() && term(ASSIGN) && expr_list();
}

bool var_spec() {
    TokenNode *save = next;
    return (next = save, var_spec1())
           || (next = save, var_spec2());
}

/**
 * GRAMMAR:
 * VAR_SPECS_LIST1 = VAR_SPEC VAR_SPEC_LIST
 */
bool var_specs_list1() {
    return var_spec() && var_specs_list();
}
/**
 * GRAMMAR:
 * WAR_SPECS_LIST1 = VAR SPEC VAR_SPEC_LIST | EPSILON
 */
bool var_specs_list() {
    TokenNode *save = next;
    return (next = save, var_decl())
           || (next = save, true);
}

bool var_decl1() {
    return term(VAR) && term(OPEN) && var_specs_list() && term(CLOSE);
}

bool var_decl2() {
    return term(VAR) && var_spec();
}



bool var_decl() {
    TokenNode *save = next;
    return (next = save, var_decl())
           || (next = save, var_decl1())
           || (next = save, var_decl2());
}

bool declarations() {
    TokenNode *save = next;
    return (next = save, var_decl())
            ;/*TODO: || (next = save, const_decl())
            || (next = save, type_decl());*/
}

bool statement() {
    TokenNode *save = next;
    return (next = save, declarations());
}

/**
 * GRAMMAR:
 * STATEMENTS_LIST1 = STATEMENT STATEMENTS_LIST
 */
bool statements_list1() {
    return statement() && statements_list();
}


/**
 * GRAMMAR:
 * STATEMENTS_LIST =
 * STATEMENT SIATEMENTS_LIST | EPSILON
 */
bool statements_list() {
    TokenNode *save = next;
    return (next = save, statements_list1())
           || (next = save, true);
}

bool block() {
    return term(BLOPEN) && statements_list() && term(BLCLOSE);
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

/**
 * TODO: ...
 * GRAMMAR:
 * ID_LIST TYPE
 */
ParamsList *param_decl1() {
    IdsList *idsList = (IdsList *) malloc(sizeof(IdsList));
    idsList->next = NULL;
    Token token;
    token.type = TOKENNONE;
    idsList->token_info = token;

    if(!id_list(idsList)) return NULL;
    if(!type()) return NULL;

    ParamsList *paramsList = (ParamsList *) malloc(sizeof(ParamsList));
    ParamsList *tmpParam = paramsList;

    for(IdsList *tmpIdL = idsList->next; tmpIdL != NULL; tmpIdL = tmpIdL->next) {
        tmpParam->next = (ParamsList *) malloc(sizeof(ParamsList));
        tmpParam = tmpParam->next;
        tmpParam->next = NULL;

        tmpParam->var = tmpIdL->token_info;
        tmpParam->vartype = next->token;
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

bool func_body() {
    return block();
}

/**
 * GRAMMAR:
 * FUNC_DELARATION1 = func id FUNC_SIGNATURE FUNC_BODY
 */
FuncDeclNode *func_declaration1() {
    return NULL;// return term(FUNC) && term(ID) && func_signature() && func_body();
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


/**
 * DECLARATION_SECTION = FUNC_DECLARATION
 *                     | VAR_DECLARATION
 *                     | CONST_DECLARATION
 *                     | TYPE_DECLARATION
 *
 */
DeclSectionNode *top_level_declarations() {
    TokenNode *save = next;
    bool isDecl = true;

    DeclSectionNode *declSectionNode = ast_create_decl_section_node();


    while (isDecl) {
        NodeType nodeType = NODENONE;

        void *decl = (next = save, func_declaration());
        if(decl) nodeType = FUNC_DECL;

        //if(!decl) decl = (next = save, ...)


        if(decl) {
           AdvDeclList *tmp = declSectionNode->decl_list;
           while(tmp->next) tmp = tmp->next;
           tmp->next = (AdvDeclList *) malloc(sizeof(AdvDeclList));
           tmp = tmp->next;
           tmp->declaration = decl;
           tmp->type = nodeType;
           tmp->next = NULL;

           save = next; // UPDATE SAVED POINTER
        } else {
            isDecl = false; // EXIT FROM CYCLE;
        }
    }

    return declSectionNode;
 }

/**
 * GRAMMAR:
 * GO_PROGRAM = PACKAGE_DIFINITION DECLARATION_SECTION
 */
ProgramNode *go_program() {
    ProgramNode *programNode = ast_create_pogram_node();
    programNode->package_decl = package_difinition();
    programNode->decl_section = top_level_declarations();


    return programNode->decl_section && programNode->package_decl ? programNode : NULL;

}

void parser_start(TokenNode *tokens) {
    next = tokens;
    ProgramNode *root = go_program();
    printf("\n");
    if(root) {
        if(root->package_decl){
            printf("PACKAGE %s\n", root->package_decl->id.info.varname);
        }
        if(root->decl_section) {
            printf("DECLARATION SECTION\n");
            AdvDeclList *advDeclList = root->decl_section->decl_list->next;
            while(advDeclList) {
                switch (advDeclList->type) {
                    case FUNC_DECL:
                        printf("   FUNC_DECL\n");
                        FuncDeclNode *funcDeclNode = (FuncDeclNode *) advDeclList->declaration;
                        printf("       NAME: %s\n", funcDeclNode->token_info.info.varname);
                        if(funcDeclNode->params) {
                            ParamsList *paramsList = funcDeclNode->params->next;
                            printf("       VARS:\n");
                            while (paramsList) {
                                printf("          %s | %s\n", paramsList->var.info.varname, paramsList->vartype.info.varname);

                                paramsList = paramsList->next;
                            }
                        }

                        if(funcDeclNode->results) {
                            ResultsList *resultsList = funcDeclNode->results->next;
                            printf("       RES:\n");
                            while (resultsList) {
                                printf("          %s\n", resultsList->token_info.info.varname);

                                resultsList = resultsList->next;
                            }
                        }
                        break;
                }
                advDeclList = advDeclList->next;
            }

        }
    }
    // TODO: START PARSING
}
