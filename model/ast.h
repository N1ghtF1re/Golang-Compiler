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
    GO_PROGRAM,
    PACKAGE_DECL,
    DECL_SECTION,
    FUNC_DECL,
    FUNC_PARAMS,
    FUNC_RESULT,
    NODENONE
} NodeType;


typedef struct IdsList {
    Token token_info;
    struct IdsList *next;
} IdsList;

typedef struct PackageNode {
    NodeType type;
    Token id;
} PackageNode;

typedef struct ResultsList  {
    Token token_info;
    struct ResultsList* next;
} ResultsList;

typedef struct ParamsList  {
    Token var;
    Token vartype;
    struct ParamsList* next;
} ParamsList;

typedef struct FuncBody {

} FuncBody;

typedef struct FuncDeclNode {
    NodeType type;
    Token token_info;
    ParamsList *params;
    ResultsList *results;
    FuncBody *body;
} FuncDeclNode;

typedef struct AdvDeclList {
    NodeType type;
    void* declaration;
    struct AdvDeclList *next;
} AdvDeclList;

typedef struct DeclSectionNode {
    NodeType type;
    AdvDeclList *decl_list;
} DeclSectionNode;

typedef struct ProgramNode{
    NodeType  type;
    PackageNode *package_decl;
    DeclSectionNode *decl_section;
} ProgramNode;

/** Side NODES **/

typedef struct FuncSignatureSideNode {
    ParamsList *params;
    ResultsList *results;
} FuncSignatureSideNode;
FuncSignatureSideNode *ast_create_signature_side_node();

/** Side Nodes **/

ProgramNode *ast_create_pogram_node();
PackageNode *ast_create_package_node();
DeclSectionNode *ast_create_decl_section_node();
FuncDeclNode *ast_create_func_decl_node();






#endif //GOCOMPILER_AST_H
