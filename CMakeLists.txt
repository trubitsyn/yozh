cmake_minimum_required(VERSION 3.12)
project(yozh C)
set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} src/builtins.c src/lexer.c src/parser.c src/yozh.c src/main.c)

target_include_directories(${PROJECT_NAME} PUBLIC include)
