//
// Created by alex on 15.12.18.
//

#include "ast.h"

ProgramNode *ast_create_pogram_node() {
    ProgramNode *programNode = (ProgramNode *) malloc(sizeof(ProgramNode));
    programNode->type = GO_PROGRAM;
    programNode->package_decl = NULL;
    programNode->decl_section = NULL;

    return programNode;
}

PackageNode *ast_create_package_node() {
    PackageNode *packageNode = (PackageNode *) malloc(sizeof(PackageNode));
    packageNode->type = PACKAGE_DECL;

    return packageNode;
}

DeclSectionNode *ast_create_decl_section_node() {
    DeclSectionNode *declSectionNode = (DeclSectionNode *) malloc(sizeof(DeclSectionNode));
    declSectionNode->type = DECL_SECTION;

    AdvDeclList *advDeclList = (AdvDeclList*) malloc(sizeof(AdvDeclList));
    advDeclList->next = NULL;
    advDeclList->type = NODENONE;
    advDeclList->declaration = NULL;

    declSectionNode->decl_list = advDeclList;

    return declSectionNode;
}

FuncDeclNode *ast_create_func_decl_node() {
    FuncDeclNode *funcDeclNode = (FuncDeclNode *) malloc(sizeof(FuncDeclNode));
    funcDeclNode->type = FUNC_DECL;
    funcDeclNode->body = NULL;

    ParamsList *paramsList = (ParamsList *) malloc(sizeof(ParamsList));
    paramsList->next = NULL;


    ResultsList *resultsList = (ResultsList *) malloc(sizeof(ResultsList));
    resultsList->next = NULL;

    funcDeclNode->params = paramsList;
    funcDeclNode->results = resultsList;

    return funcDeclNode;
}

FuncSignatureSideNode *ast_create_signature_side_node() {
    FuncSignatureSideNode *sideNode = (FuncSignatureSideNode*) malloc((sizeof(FuncSignatureSideNode)));
    sideNode->params = NULL;
    sideNode->results = NULL;

    return sideNode;
}

BlockNode *ast_create_block_node() {
    BlockNode *blockNode = (BlockNode*) malloc(sizeof(BlockNode));
    blockNode->statements_list = NULL;
    blockNode->type = BLOCK;
}

VarDeclNode *ast_create_var_decl_node() {
    VarDeclNode *varDeclNode = (VarDeclNode*) malloc(sizeof(VarDeclNode));
    varDeclNode->type = VAR_DECL;
    varDeclNode->vars = NULL;
    varDeclNode->expressions = NULL;
}

ExpressionNode *ast_create_expr_node(void *expression) {
    ExpressionNode *expressionNode = (ExpressionNode *) malloc(sizeof(ExpressionNode));
    expressionNode->type = NODENONE;

    expressionNode->expr = expression;

    return expressionNode;
}

ExpressionNode *ast_create_postfix_expr_node(ExpressionNode *postfix, ExpressionNode *operand) {


    PostfixExprNode *postfixExprNode = (PostfixExprNode *) malloc(sizeof(PostfixExprNode));
    postfixExprNode->operand = operand;
    postfixExprNode->postfix = postfix;

    ExpressionNode *expressionNode = ast_create_expr_node(postfixExprNode);
    expressionNode->type = POSTFIX_EXPR;

    return expressionNode;
}

ExpressionNode *ast_create_expr_token_node(Token token) {
    OnlyTokenNode *onlyTokenNode = malloc(sizeof(OnlyTokenNode));
    onlyTokenNode->token = token;

    ExpressionNode *expressionNode = ast_create_expr_node(onlyTokenNode);
    expressionNode->type = ONLY_TOKEN;

    return expressionNode;
}

ExpressionNode *ast_create_unar_expr_node(ExpressionNode *operand, Token operator) {
    UnarExprNode *unarExprNode = (UnarExprNode *) malloc(sizeof(UnarExprNode));
    unarExprNode->operand = operand;
    unarExprNode->operator = operator;

    ExpressionNode *expressionNode = ast_create_expr_node(unarExprNode);
    expressionNode->type = UNAR_EXPR;

    return expressionNode;
}
ExpressionNode *ast_create_binar_expr_node(ExpressionNode *l, ExpressionNode *r, Token operator) {
    BinarExprNode *binarExprNode = (BinarExprNode *) malloc(sizeof(BinarExprNode));
    binarExprNode->L = l;
    binarExprNode->R = r;
    binarExprNode->operator = operator;

    ExpressionNode *expressionNode = ast_create_expr_node(binarExprNode);
    expressionNode->type = BINAR_EXPR;

    return expressionNode;
}
/**
 * LISTS:
**/
AdvDeclList *ast_decl_list_push(AdvDeclList *list, NodeType nodeType, void* decl) {
    AdvDeclList *tmp = list;
    while(tmp->next) tmp = tmp->next;
    tmp->next = (AdvDeclList *) malloc(sizeof(AdvDeclList));
    tmp = tmp->next;
    tmp->declaration = decl;
    tmp->type = nodeType;
    tmp->next = NULL;

    return tmp;
}

StatementsList *ast_statements_list_create() {
    StatementsList *statementsList = (StatementsList *) malloc(sizeof(StatementsList));
    statementsList->next = NULL;
    statementsList->statement = NULL;
    statementsList->type = NODENONE;

    return statementsList;
}

VarDeclSideList *ast_var_decl_side_list_create() {
    VarDeclSideList *varDeclSideList = (VarDeclSideList *) malloc(sizeof(VarDeclSideList));

    varDeclSideList->next = NULL;
    varDeclSideList->decl = NULL;

    return varDeclSideList;
}

IdsList *ast_id_lsit_create() {
    IdsList *idsList = (IdsList *) malloc(sizeof(IdsList));
    idsList->next = NULL;
    Token token;
    token.type = TOKENNONE;
    idsList->token_info = token;

    return idsList;
}

ParamsList *cpyIdListIntoParamList(IdsList *idsList) {
    ParamsList *paramsList = (ParamsList *) malloc(sizeof(ParamsList));
    ParamsList *tmpParam = paramsList;

    for(IdsList *tmpIdL = idsList->next; tmpIdL != NULL; tmpIdL = tmpIdL->next) {
        tmpParam->next = (ParamsList *) malloc(sizeof(ParamsList));
        tmpParam = tmpParam->next;
        tmpParam->next = NULL;

        tmpParam->var = tmpIdL->token_info;
        Token token;
        token.type = TOKENNONE;
        token.info.varname = "Undefined";
    }

    return paramsList;
}



ExprList *ast_create_expr_list() {
    ExprList *exprList = (ExprList *) malloc(sizeof(ExprList));
    exprList->next = NULL;
    exprList->expression = NULL;
}