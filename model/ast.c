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