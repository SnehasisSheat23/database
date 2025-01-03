cmake_minimum_required(VERSION 3.30)
project(database)

set(CMAKE_CXX_STANDARD 20)

add_executable(database
        src/Tokenizer/token.cpp
        src/Parser/parser.cpp
        )
