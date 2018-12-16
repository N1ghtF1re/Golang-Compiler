//
// Created by alex on 16.12.18.
//

#ifndef GOCOMPILER_FUNCTIONS_H
#define GOCOMPILER_FUNCTIONS_H

#include "../parser.h"
#include "../common/common.h"

FuncDeclNode *func_declaration();
BlockNode *func_body();
FuncSignatureSideNode *func_signature();
ResultsList *func_result();
ParamsList *func_params();
bool func_paramslist(ParamsList *tmpNode);


#endif //GOCOMPILER_FUNCTIONS_H


