//include statements
#include "lexer/lexer.h"
#include <cctype>
#include <stdexcept>
#include <unordered_set>
#include <utility>

//gotta open my namespace of course
namespace split {
    //actual lexer constructor
    Lexer::Lexer(std::string text) : text_(std::move(text)) {}
    
    //function for the actual lexing
    std::vector<Token> Lexer::lex() {
        //lit just returns an empty vector for now
        std::vector<Token> v;
        return v;
    }
}



