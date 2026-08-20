add_executable(splt
    src/main.cpp
    src/lexer/lexer.cpp
)
target_include_directories(splt PRIVATE src)
target_compile_options(spl PRIVATE -Wall -Wextra)