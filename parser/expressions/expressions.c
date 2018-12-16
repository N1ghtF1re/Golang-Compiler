//
// Created by alex on 16.12.18.
//

#include "expressions.h"

bool array_or_cut();
ExpressionNode *mul_expr();
ExpressionNode *add_expr();


ExpressionNode *postfix_expr1() {
    if(!term(ID)) return NULL;
    Token idtoken = next->token;
    if(!term(PLUSPLUS)) return NULL;

    ExpressionNode *postfix = ast_create_expr_token_node(next->token);
    ExpressionNode *operand = ast_create_expr_token_node(idtoken);

    return ast_create_postfix_expr_node(postfix,operand);
}
ExpressionNode *postfix_expr2() {
    if (!term(ID)) return NULL;
    Token idtoken = next->token;
    if (!term(MINUSMINUS)) return NULL;

    ExpressionNode *postfix = ast_create_expr_token_node(next->token);
    ExpressionNode *operand = ast_create_expr_token_node(idtoken);

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
    if(isId) return ast_create_expr_token_node(next->token);

    bool isLiteral = (next = save, literal());
    if(isLiteral) return ast_create_expr_token_node(next->token);

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