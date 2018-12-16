//
// Created by alex on 13.12.18.
//
#include <stdio.h>
#include <memory.h>
#include "parser.h"

TokenNode* next; // Pointer to the next token

#define lowpr_expr add_expr

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
StatementsList *statements_list();
VarDeclSideList *var_specs_list();
bool expr_list();
VarDeclSideList *var_decl();
ExpressionNode *expression();
ExpressionNode *postfix_expr();
bool array_or_cut();
bool func_call();
ExpressionNode *mul_expr();
ExpressionNode *add_expr();

/**
 * GRAMMAR:
 * PACKAGE DIFINITION = package identificator
 *
 * TODO: передалать узел с типом на нетерминальный
 */
PackageNode *package_difinition() {
    if(!term(PACKAGE)) return NULL;
    if(!term(ID)) return NULL;
    PackageNode *packageNode = ast_create_package_node();
    packageNode->id = next->token;
    return packageNode;
}


bool type() {
    TokenNode *save = next;
    return (next = save, term(PRIM_TYPE))
           || (next = save, term(ID));
}

bool literal() {
    TokenNode *save = next;
    return (next = save, term(STRING_LIT))
           || (next = save, term(INT_LIT))
           || (next = save, term(CHAR_LIT))
           || (next = save, term(FLOAT_LIT));
}

ExpressionNode *postfix_expr1() {
    if(!term(ID)) return NULL;
    Token idtoken = next->token;
    if(!term(PLUSPLUS)) return NULL;

    ExpressionNode *postfix = ast_create_token_node(next->token);
    ExpressionNode *operand = ast_create_token_node(idtoken);

    return ast_create_postfix_expr_node(postfix,operand);
}
ExpressionNode *postfix_expr2() {
    if (!term(ID)) return NULL;
    Token idtoken = next->token;
    if (!term(MINUSMINUS)) return NULL;

    ExpressionNode *postfix = ast_create_token_node(next->token);
    ExpressionNode *operand = ast_create_token_node(idtoken);

    return ast_create_postfix_expr_node(postfix, operand);
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
ExpressionNode *postfix_expr5() {
    TokenNode *save = next;

    OnlyTokenNode *onlyTokenNode = NULL;

    bool isId = (next = save, term(ID));
    if(isId) return ast_create_token_node(next->token);

    bool isLiteral = (next = save, literal());
    if(isLiteral) return ast_create_token_node(next->token);

    return NULL;

}

/**
 * GRAMMAR:
 * POSTFIX_EXPRESSION = ID ++
 *                  |   ID --
 *                  |   ID FUNC_CALL
 *                  |   ID ARRAY_OR_CUT
 *                  |   ID
 */
ExpressionNode *postfix_expr() {
    TokenNode *save = next;

    ExpressionNode *expressionNode = NULL;

    if(!expressionNode) expressionNode = (next = save, postfix_expr1());
    if(!expressionNode) expressionNode = (next = save, postfix_expr2());
    //if(!expressionNode) expressionNode = (next = save, postfix_expr3());
    //if(!expressionNode) expressionNode = (next = save, postfix_expr4());
    if(!expressionNode) expressionNode = (next = save, postfix_expr5());


    return expressionNode;
    /*
    return (next = save, postfix_expr1())
           || (next = save, postfix_expr2())
           // TODO :|| (next = save, postfix_expr3())
           // TODO || (next = save, postfix_expr4())
           || (next = save, postfix_expr5());
    */
}

ExpressionNode *unar_operator1() {
    TokenNode *save = next;

    bool isUnarOperator = (next = save, term(PLUS)) || (next = save, term(MINUS)) || (next = save, term(CARET));

    Token operator;

    if(isUnarOperator) {
        operator = next->token;
    } else {
        next = save;
        return NULL;
    }

    ExpressionNode *expressionNode = postfix_expr();
    if(!expressionNode) return NULL;


    return ast_create_unar_expr_node(expressionNode, operator);
}

ExpressionNode *unar_operator2() {
    return postfix_expr();
}

/**
 * GRAMMAR
 * UNAR 	= + POSTFIX
	        | - POSTFIX
	        | ^ POSTFIX
	        | POSTFIX
 */
ExpressionNode *unar_operator() {
    TokenNode *save = next;

    ExpressionNode *expressionNode = NULL;
    if(!expressionNode) expressionNode = (next = save, unar_operator1());
    if(!expressionNode) expressionNode = (next = save, unar_operator2());

    return expressionNode;
}

ExpressionNode *mul_expr1() {
    ExpressionNode *unarExpr = unar_operator();
    if(!unarExpr) return NULL;

    TokenNode *save = next;

    bool isMulOperator = (next = save, term(DIV)) || (next = save, term(MOD)) || (next = save, term(MUL));

    if(!isMulOperator) return NULL;

    Token token = next->token;

    ExpressionNode *mulExpr = mul_expr();

    if(!mulExpr) return NULL;

    return ast_create_binar_expr_node(unarExpr, mulExpr, token);
}

ExpressionNode *mul_expr2() {
    return unar_operator();
}

ExpressionNode *mul_expr3() {
    if(!term(OPEN)) return NULL;
    ExpressionNode *lowPrExpr = lowpr_expr(); // Expression with lower priority operator
    if(!term(CLOSE)) return NULL;

    TokenNode *save = next;

    bool isMulOperator = (next = save, term(DIV)) || (next = save, term(MOD)) || (next = save, term(MUL));

    if(!isMulOperator) return NULL;

    Token token = next->token;

    ExpressionNode *mulExpr = mul_expr();
    if(!mulExpr) return NULL;

    return ast_create_binar_expr_node(lowPrExpr, mulExpr, token);
}

ExpressionNode *mul_expr4() {
    if(!term(OPEN)) return NULL;
    ExpressionNode *lowPrExpr = lowpr_expr();
    if(!term(CLOSE)) return NULL;

    return lowPrExpr;
}

/**
 * GRAMMAR:
 * MUL	= UNAR * MUL
	    | UNAR / MUL
	    | UNAR % MUL
	    | UNAR
	    | (LOGOR) * MUL
	    | (LOGOR) / MUL
	    | (LOGOR) % MUL
	    | (LOGOR)
 */
ExpressionNode *mul_expr() {
    TokenNode *save = next;

    ExpressionNode *mulExpr = NULL;
    if(!mulExpr) mulExpr = (next = save, mul_expr1());
    if(!mulExpr) mulExpr = (next = save, mul_expr2());
    if(!mulExpr) mulExpr = (next = save, mul_expr3());
    if(!mulExpr) mulExpr = (next = save, mul_expr4());

    return mulExpr;
}

ExpressionNode *add_expr1() {
    ExpressionNode *mulExpr = mul_expr();
    if(!mulExpr) return NULL;

    TokenNode *save = next;

    bool isSumOperator = (next = save, term(PLUS)) || (next = save, term(MINUS));

    if(!isSumOperator) return NULL;
    Token token = next->token;

    ExpressionNode *addExpr = add_expr();

    return ast_create_binar_expr_node(mulExpr, addExpr, token);
}

ExpressionNode *add_expr2() {
    return mul_expr();
}


ExpressionNode *add_expr() {
    TokenNode *save = next;

    ExpressionNode *addExpr = NULL;
    if(!addExpr) addExpr = (next = save, add_expr1());
    if(!addExpr) addExpr = (next = save, add_expr2());

    return addExpr;
}

/**
 * TODO: _
 */
ExpressionNode *expression() {
    // TODO : SHIFTS, RELATIONS etc
    return lowpr_expr();
}


/**
 * GRAMMAR:
 * VAR_SPECS_LIST1 = VAR_SPEC VAR_SPEC_LIST
 */
bool expr_list1(ExprList *exprList) {
    ExpressionNode *expressionNode = expression();
    if(!expressionNode) return false;
    if(!term(COMMA)) return false;

    exprList->next = ast_create_expr_list();
    exprList->next->expression = expressionNode;
    return expr_list(exprList->next);
}

ExprList *expr_list2(ExprList *exprList) {
    exprList->next = ast_create_expr_list();
    exprList->next->expression = expression();
    return (exprList->next->expression != NULL) ? exprList : NULL;
}

/**
 * GRAMMAR:
 * EXPR_LIST = EXPR_LIST | EPSILON
 */
bool expr_list(ExprList *exprList) {
    TokenNode *save = next;
    return (next = save, exprList->next = NULL, expr_list1(exprList))
           || (next = save, exprList->next = NULL, expr_list2(exprList));
}

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

StatementsList *statement() {
    TokenNode *save = next;
    StatementsList *currProduction;

    currProduction = NULL;
    if(!currProduction) currProduction = (next = save, declarations());

    return currProduction;
}



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
        if(decl) {
            nodeType = FUNC_DECL;
            ast_decl_list_push(declSectionNode->decl_list, nodeType, decl);
        }

        if(!decl) {
            decl = (next = save, var_decl());
            if (decl) {
                VarDeclSideList *tmp = decl;
                nodeType = VAR_DECL;
                for (tmp = tmp->next; tmp != NULL; tmp = tmp->next) {
                    ast_decl_list_push(declSectionNode->decl_list, nodeType, tmp->decl);
                }

            }
        }


        if(decl) {
            save = next; // UPDATE SAVED POINTER
        } else {
            next = save;
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

char* getTokenInfo(Token token) {
    char *outstr = (char*) malloc(255 * sizeof(char));
    switch (token.type) {
        case ID:
            sprintf(outstr, "ID. NAME: %s", token.info.varname);
            break;
        case STRING_LIT:
            sprintf(outstr, "STRING_LIT. VALUE: %s", token.info.value.strvalue);
            break;
        case CHAR_LIT:
            sprintf(outstr, "CHAR_LIT. VALUE: %c", token.info.value.intfalue);
            break;
        case FLOAT_LIT:
            sprintf(outstr, "FLOAT_LIT. VALUE: %f", token.info.value.floatvalue);
            break;
        case INT_LIT:
            sprintf(outstr, "INT_LIT. VALUE: %d", token.info.value.intfalue);
            break;
        case PRIM_TYPE:
            sprintf(outstr, "PRIM_TYPE. VALUE: %s", token.info.varname);
            break;
        case PLUS:
            outstr = "+";
            break;
        case MINUS:
            outstr = "-";
            break;
        case MUL:
            outstr = "*";
            break;
        case DIV:
            outstr = "/";
            break;
        case MOD:
            outstr = "%";
            break;
        case PLUSPLUS:
            outstr = "++";
            break;
        case MINUSMINUS:
            outstr = "--";
            break;
    }

    return outstr;
}

void outPutExpression(ExpressionNode* node, char *startchars) {
    char *newstartchar = (char *) malloc((strlen(startchars) + 4)*sizeof(char));
    sprintf(newstartchar, "%s%s", startchars, "   ");
    switch (node->type) {
        case ONLY_TOKEN: {
            OnlyTokenNode *tokenNode = (OnlyTokenNode *) node->expr;
            printf("%s%s\n", startchars, getTokenInfo(tokenNode->token));
            break;
        }
        case BINAR_EXPR: {
            BinarExprNode *binarExprNode = (BinarExprNode *) node->expr;
            printf("%s%s\n", startchars, getTokenInfo(binarExprNode->operator));

            outPutExpression(binarExprNode->L, newstartchar);
            outPutExpression(binarExprNode->R, newstartchar);
            break;
        }
        case UNAR_EXPR: {
            UnarExprNode *unarExprNode = (UnarExprNode *) node->expr;
            printf("%s%s\n", startchars, getTokenInfo(unarExprNode->operator));
            sprintf(newstartchar, "%s%s", startchars, "   ");
            outPutExpression(unarExprNode->operand, newstartchar);
            break;
        }
        case POSTFIX_EXPR: {
            PostfixExprNode *postfixExprNode = (PostfixExprNode *) node->expr;
            outPutExpression(postfixExprNode->postfix, startchars);
            sprintf(newstartchar, "%s%s", startchars, "   ");
            outPutExpression(postfixExprNode->operand, newstartchar);
        }
    }
}

void outputVarDecl(VarDeclNode *varDeclNode, char *startchars) {
    char *startchars1 = (char *) malloc((strlen(startchars) + 4)*sizeof(char));
    char *startchars2 = (char *) malloc((strlen(startchars) + 7)*sizeof(char));
    sprintf(startchars1, "%s%s", startchars, "   ");
    sprintf(startchars2, "%s%s", startchars, "      ");


    printf("%s%s", startchars, "VAR_DECL\n");
    ParamsList *paramsList = varDeclNode->vars->next;
    printf("%s%s", startchars1, "LEFT PART\n");
    while (paramsList) {
        printf("%s%s | %s\n", startchars2, paramsList->var.info.varname, paramsList->vartype.info.varname);

        paramsList = paramsList->next;
    }
    if(varDeclNode->expressions) {
        printf("%s%s", startchars1, "RIGHT PART\n");
        ExprList *exprList = varDeclNode->expressions->next;
        while (exprList) {
            outPutExpression(exprList->expression, startchars2);

            exprList = exprList->next;
        }
    } else {
        printf("%s%s", startchars1, "WITHOUT RIGHT PART\n");
    }
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
                    case FUNC_DECL: {
                        printf("    FUNC_DECL\n");
                        FuncDeclNode *funcDeclNode = (FuncDeclNode *) advDeclList->declaration;
                        printf("       NAME: %s\n", funcDeclNode->token_info.info.varname);
                        if (funcDeclNode->params) {
                            ParamsList *paramsList = funcDeclNode->params->next;
                            printf("       VARS:\n");
                            while (paramsList) {
                                printf("          %s | %s\n", paramsList->var.info.varname,
                                       paramsList->vartype.info.varname);

                                paramsList = paramsList->next;
                            }
                        }

                        if (funcDeclNode->results) {
                            ResultsList *resultsList = funcDeclNode->results->next;
                            printf("       RES:\n");
                            while (resultsList) {
                                printf("          %s\n", resultsList->token_info.info.varname);

                                resultsList = resultsList->next;
                            }
                        }

                        if (funcDeclNode->body) {
                            StatementsList *statementsList = funcDeclNode->body->statements_list->next;
                            printf("       BODY:\n");
                            while (statementsList) {
                                switch (statementsList->type) {
                                    case VAR_DECL:
                                        outputVarDecl(statementsList->statement, "          ");
                                        break;
                                }

                                statementsList = statementsList->next;
                            }
                        }
                        break;
                    }
                    case VAR_DECL: {
                        VarDeclNode *varDeclNode1 = (VarDeclNode *) advDeclList->declaration;
                        outputVarDecl(varDeclNode1, "    ");
                        break;
                    }
                }
                advDeclList = advDeclList->next;
            }

        }
    }
    // TODO: START PARSING
}
