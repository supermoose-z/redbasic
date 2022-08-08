cmake_minimum_required(VERSION 3.10)

project(RedBASIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LEXER_FILES lexer/lexer.cpp)
set(VM_FILES vm/scope.cpp vm/vm.cpp vm/value.cpp vm/var.cpp)
set(AST_FILES ast/valuenode.cpp ast/printnode.cpp ast/assignnode.cpp ast/binopnode.cpp)
set(PARSER_FILES parser/parser.cpp)

add_executable(rbasic main.cpp error.cpp ${LEXER_FILES} ${VM_FILES} ${AST_FILES} ${PARSER_FILES})
