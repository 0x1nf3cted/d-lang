cmake_minimum_required(VERSION 3.10)
project(dzair)

include_directories(include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

set(SOURCE_FILES
    src/main.c
    src/lexer.c
    src/parser.c
)

add_executable(dzair ${SOURCE_FILES})
 