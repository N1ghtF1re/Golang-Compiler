//
// Created by alex on 16.12.18.
//

#ifndef GOCOMPILER_EXPRESSIONS_H
#define GOCOMPILER_EXPRESSIONS_H

#include "../parser.h"

#define lowpr_expr add_expr // EXPRESSION WITH LOWER PRIORITY


ExpressionNode *expression();
bool expr_list(ExprList *exprList);

#endif //GOCOMPILER_EXPRESSIONS_H
