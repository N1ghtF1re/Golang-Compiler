cmake_minimum_required(VERSION 3.12)
project(GOCompiler C)

set(CMAKE_C_STANDARD 99)

add_executable(GOCompiler main.c lexer/lexer.h lexer/lexer.c model/go_types.h model/tokens.h parser/parser.h parser/parser.c model/ast.h model/ast.c parser/functions/functions.c parser/functions/functions.h parser/common/common.c parser/common/common.h parser/basicdel/basicdecl.c parser/basicdel/basicdecl.h parser/expressions/expressions.c parser/expressions/expressions.h)