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
        //create our empty vector of tokens
        std::vector<Token> v;
        
        //start going thru char by char to create tokens
        while(!atEnd()){
            //look at the current char
            char c = peek();

            if(c == ' ' || c == '\t'){
                advance(); //this way whitespace IN THE CODE is ignored so i can make as many spaces and such as i want
            }
            else if(c == '\n'){
                Token t;
                t.type = Tok::NEWLINE;
                t.line = line_;
                line_++;

                //adds this token to the array of tokens
                v.push_back(t);
                advance();
                
            }
            else if(c == '#'){
                while(peek() != '\n' && !atEnd()){
                    advance();
                }
            }
            //else pretty much covers everything right now. This will be a lot of logic later.
            else {
                advance();
            }

        }

        //now that we are outside of the loop make the end token and add it to vector
        Token t;
        t.type = Tok::END;
        t.line = line_;
        v.push_back(t);
        
        return v; //all the tokens
    }
}



