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

bool id_list(); // PREDECLARATION
bool func_paramslist();
bool func_declarations();
bool top_level_declarations();
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
 */
bool package_difinition() {
    return term(PACKAGE) && term(ID);
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
    return term(OPEN) && id_list() && term(CLOSE) && func_call();
}
bool func_call12() {
    return term(OPEN) && id_list() && term(CLOSE);
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
    return id_list() && type() && term(ASSIGN) && expr_list();
}

bool var_spec2() {
    return id_list() && term(ASSIGN) && expr_list();
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
bool id_list1() {
    return term(ID) && term(COMMA) && id_list();
}

/**
 * GRAMMAR:
 * ID_LIST1 = ID
 */
bool id_list2() {
    return term(ID);
}

bool id_list() {
    TokenNode *save = next;
    return (next = save, id_list1())
           || (next = save, id_list2());
}

/**
 * TODO: ...
 * GRAMMAR:
 * ID_LIST TYPE
 */
bool param_decl1() {
    return id_list() && type();
}

/**
 * GRAMMAR:
 * TYPE
 */
bool param_decl2() {
    return type();
}

bool param_decl() {
    TokenNode *save = next;
    return (next = save, param_decl1())
           || (next = save, param_decl2());
}

/**
 * GRAMMAR:
 * FUNC_PARAMSLIST1 = PARAM_DECL , FUNC_PARAMS
 */
bool func_paramslist1() {
    return param_decl() && term(COMMA) && func_paramslist();
}

/**
 * GRAMMAR:
 * FUNC_PARAMSLIST1 = PARAM_DECL
 */
bool func_paramslist2() {
    return param_decl();
}

bool func_paramslist() {
    TokenNode *save = next;
    return (next = save, func_paramslist1())
           || (next = save, func_paramslist2())
           || (next = save, true); // Params - optional
}

/**
 * GRAMMAR:
 * ( FUNC_PARAMSLIST )
 */
bool func_params() {
    return term(OPEN) && func_paramslist() && term(CLOSE);
}


bool func_result1() {
    return func_params();
}
bool func_result2() {
    return type();
}

bool func_result() {
    TokenNode *save = next;
    return (next = save, func_result1())
           || (next = save, func_result2())
           || (next = save, true); // RESULT - optional
}

/**
 * GRAMMAR:
 * FUNC_PARAMS FUNC_RESULT
 * @return
 */
bool func_signature1() {
    return func_params() && func_result();
}

/**
 * GRAMMAR:
 * FUNC_PARAMS
 * @return
 */
bool func_signature2() {
    return func_params();
}

bool func_signature() {
    TokenNode *save = next;
    return (next = save, func_signature1())
           || (next = save, func_signature2());
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
bool func_declaration1() {
    return false;// return term(FUNC) && term(ID) && func_signature() && func_body();
}

/**
 * GRAMMAR:
 * FUNC_DELARATION2 = func id FUNC_SIGNATURE
 */
bool func_declaration2() {
    if(term(FUNC) && term(ID) && func_signature()) {
        printf("\nKEK!\n");
        return true;
    };
    printf("\nMEM\n");
    return false;
}

bool func_declaration() {
    TokenNode *save = next;

    return (next = save, func_declaration1())
           || (next = save, func_declaration2());
}


bool func_declarations1() {
    return func_declaration() && top_level_declarations();
}


bool func_declarations() {
    TokenNode *save = next;
    return (next = save, func_declarations1())
           || (next = save, true);

}

/**
 * DECLARATION_SECTION = FUNC_DECLARATION
 *                     | VAR_DECLARATION
 *                     | CONST_DECLARATION
 *                     | TYPE_DECLARATION
 *
 */
bool top_level_declarations() {
    TokenNode *save = next;
    return (next = save, func_declarations())
           || (next = save, false);
}

/**
 * GRAMMAR:
 * GO_PROGRAM = PACKAGE_DIFINITION DECLARATION_SECTION
 */
bool go_program() {
    return package_difinition() && top_level_declarations();
}

void parser_start(TokenNode *tokens) {
    next = tokens;
    printf("\n%d\n", go_program());
    // TODO: START PARSING
}
