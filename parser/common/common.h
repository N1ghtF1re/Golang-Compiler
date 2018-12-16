//
// Created by alex on 16.12.18.
//

#ifndef GOCOMPILER_COMMON_H
#define GOCOMPILER_COMMON_H

#include "../parser.h"


BlockNode *block();
bool id_list(IdsList *tmpNode);
ParamsList *param_decl();
StatementsList *statement();
StatementsList *statements_list();

#endif //GOCOMPILER_COMMON_H
