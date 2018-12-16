//
// Created by alex on 13.12.18.
//
#include <stdio.h>
#include <memory.h>
#include "parser.h"

#include "common/common.h"
#include "basicdel/basicdecl.h"
#include "functions/functions.h"
#include "expressions/expressions.h"



/**
 * Checks the token to be transferred and the next token from the token stream for equivalence
 * @param token Token struct
 * @return true if tokens are equivalent
 */
bool term (TokenType token) {
    if(next ->next == NULL) return false;
    return (next = next->next)->token.type == token;
}


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
