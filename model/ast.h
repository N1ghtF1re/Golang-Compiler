//
// Created by alex on 14.12.18.
//

/**
 * ABSTRACT SYNTAX TREE NODES
 */
#ifndef GOCOMPILER_AST_H
#define GOCOMPILER_AST_H

#include <stdlib.h>
#include "../model/tokens.h"

typedef enum NodeType {
    NODENONE,
    GO_PROGRAM,
    PACKAGE_DECL,
    DECL_SECTION,
    FUNC_DECL,
    FUNC_PARAMS,
    FUNC_RESULT,
    FUNC_BODY,
    BLOCK,
    VAR_DECL,
    CONST_DECL,
    POSTFIX_EXPR,
    UNAR_EXPR,
    BINAR_EXPR,
    ONLY_TOKEN,
    TYPE_DECL

} NodeType;

typedef struct _OnlyTokenNode {
    Token token;
} OnlyTokenNode;

typedef struct _ExpressionNode {
    NodeType type;
    void *expr;
} ExpressionNode;

typedef struct _BinarExprNode {
    ExpressionNode *L;
    ExpressionNode *R;

    Token operator;
} BinarExprNode;

typedef struct _UnarExprNode {
    ExpressionNode *operand;
    Token operator;
} UnarExprNode;

typedef struct _PostfixExprNode {
    ExpressionNode *operand;
    ExpressionNode *postfix;
} PostfixExprNode;



typedef struct _ExprList {
    ExpressionNode *expression;
    struct _ExprList *next;
} ExprList;

typedef struct _ParamsList  {
    Token var;
    Token vartype;
    struct _ParamsList* next;
} ParamsList;


typedef struct _VarDeclNode {
    NodeType type;
    ParamsList *vars;
    ExprList *expressions;
} VarDeclNode;

typedef struct _VarDeclSideList {
    VarDeclNode *decl;
    struct _VarDeclSideList *next;
} VarDeclSideList;

typedef struct _IdsList {
    Token token_info;
    struct _IdsList *next;
} IdsList;

typedef struct _PackageNode {
    NodeType type;
    Token id;
} PackageNode;

typedef struct _ResultsList  {
    Token token_info;
    struct _ResultsList* next;
} ResultsList;


typedef struct _StatementsList {
    NodeType type;
    void *statement;
    struct _StatementsList *next;
} StatementsList;

typedef struct _BlockNode {
    NodeType type;
    StatementsList *statements_list;
} BlockNode;

typedef struct _FuncDeclNode {
    NodeType type;
    Token token_info;
    ParamsList *params;
    ResultsList *results;
    BlockNode *body;
} FuncDeclNode;

typedef struct _AdvDeclList {
    NodeType type;
    void* declaration;
    struct _AdvDeclList *next;
} AdvDeclList;

typedef struct _DeclSectionNode {
    NodeType type;
    AdvDeclList *decl_list;
} DeclSectionNode;

typedef struct _ProgramNode{
    NodeType  type;
    PackageNode *package_decl;
    DeclSectionNode *decl_section;
} ProgramNode;

/** Side NODES **/

typedef struct _FuncSignatureSideNode {
    ParamsList *params;
    ResultsList *results;
} FuncSignatureSideNode;
FuncSignatureSideNode *ast_create_signature_side_node();

/** Side Nodes **/

ProgramNode *ast_create_pogram_node();
PackageNode *ast_create_package_node();
DeclSectionNode *ast_create_decl_section_node();
FuncDeclNode *ast_create_func_decl_node();
BlockNode *ast_create_block_node();
VarDeclNode *ast_create_var_decl_node();
ExpressionNode *ast_create_expr_node(void *expression);
ExpressionNode *ast_create_postfix_expr_node(ExpressionNode *postfix, ExpressionNode *operand);
ExpressionNode *ast_create_expr_token_node(Token token);
ExpressionNode *ast_create_unar_expr_node(ExpressionNode *operand, Token operator);
ExpressionNode *ast_create_binar_expr_node(ExpressionNode *l, ExpressionNode *r, Token operator);

// LISTS:
AdvDeclList *ast_decl_list_push(AdvDeclList *list, NodeType nodeType, void* decl);
IdsList *ast_id_lsit_create();
StatementsList *ast_statements_list_create();
VarDeclSideList *ast_var_decl_side_list_create();
ExprList *ast_create_expr_list();

ParamsList *cpyIdListIntoParamList(IdsList *idsList);


#endif //GOCOMPILER_AST_H
