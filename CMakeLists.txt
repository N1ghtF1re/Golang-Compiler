cmake_minimum_required(VERSION 3.12)
project(GOCompiler C)

set(CMAKE_C_STANDARD 99)

add_executable(GOCompiler main.c lexer/lexer.h lexer/lexer.c go_types.h)