//include statements
#pragma once
#include <string>
#include <vector>
#include "token.h"

namespace split {
    //the lexer class
    class Lexer {
        private:
            //what the lexer needs to remember as it steps thru the code
            std::string text_; //the actual text of the script
            size_t pos_ = 0; //position (where in the text)
            int line_ = 1; //what line im on, not nessecary but i want it
        public:
            //methods
            //constructor
            explicit Lexer(std::string text); //header is just a shape file, so no body
            std::vector<Token> reader(); //entry point method that does the work
    };
}