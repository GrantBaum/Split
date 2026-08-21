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

            //some helper methods for lexing
            bool atEnd() const {
                if(pos_ >= text_.size()){
                    return true;
                }
                else return false;
            }
            //look at current char WITHOUT MOVING
            char peek() const {
                if(atEnd()){
                    return '\0';
                }
                else {
                    return text_[pos_];
                }
            }
            //look at the char one ahead of current position
            char peekNext() const {
                //implement
            }
            //look at current char and then step forward, in that order
            char advance() {
                char temp = peek();
                pos_++;
                return temp;
            }
            
        public:
            //methods
            //constructor
            explicit Lexer(std::string text); //header is just a shape file, so no body
            std::vector<Token> lex(); //entry point method that does the work of the lexing
    };
}