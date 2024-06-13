cmake_minimum_required(VERSION 3.20)

project(COMPILER VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(compiler
        imp_test.cpp
        imp.cpp
        imp_interpreter.cpp
        imp_parser.cpp
        imp_printer.cpp
        imp_typechecker.cpp
        imp_value.cpp
        #imp_test.cpp
        #svm.cpp
        #svm_parser.cpp
        #svm_run.cpp
        )