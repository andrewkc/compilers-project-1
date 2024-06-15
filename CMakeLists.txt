cmake_minimum_required(VERSION 3.22)

project(compiler VERSION 1.0.0) # MAJOR.MINOR.PATCH

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(imp
  tests/imp_test.cpp
  imp_ast.cpp
  imp_parser.cpp
  utils/imp_value.cpp
  visitors/imp_printer.cpp
  visitors/imp_interpreter.cpp
)
  
add_executable(compiler
  imp_compiler.cpp
  imp_ast.cpp
  imp_parser.cpp
  visitors/imp_printer.cpp
  visitors/imp_typechecker.cpp
  visitors/imp_interpreter.cpp
  visitors/imp_codegenerator.cpp
  utils/imp_value.cpp
)
  
add_executable(svm
  svm_run.cpp
  svm_ast.cpp
  svm_parser.cpp
)