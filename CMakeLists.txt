cmake_minimum_required(VERSION 3.16)
project(split CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(splt
  src/main.cpp
  src/lexer/lexer.cpp
)
target_include_directories(splt PRIVATE src)
target_compile_options(splt PRIVATE -Wall -Wextra)