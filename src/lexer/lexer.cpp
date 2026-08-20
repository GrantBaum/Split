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

            //handles whitespace
            if(c == ' ' || c == '\t'){
                advance(); 
            }
            //handles newlines
            else if(c == '\n'){
                Token t;
                t.type = Tok::NEWLINE;
                t.line = line_;
                line_++;

                //adds this token to the array of tokens
                v.push_back(t);
                advance();
                
            }
            //handles comments
            else if(c == '#'){

                while(peek() != '\n' && !atEnd()){
                    advance();
                }
            }
            //this one handles IDENTS, which are words that are like var names and stuff
            else if(std::isalpha(c) || c == '_'){

                std::string str;

                //keep consuming chars and appending them onto the end of my string
                while(isalnum(peek()) || peek() == '_'){
                    str += advance();
                }
                //create the token
                Token t;
                t.type = Tok::IDENT;
                t.text = str;
                t.line = line_;

                //push it into the vector
                v.push_back(t);
            }
            //handles string literals
            else if(c == '"') {

                //need to get off the initial quote mark so just advance right away
                std::string str;
                advance();
                //while the current char is not a quote and we arent at the end
                while(peek() != '"' && !atEnd()){

                    //handle newlines and ending
                    if(peek() == '\n'){
                        throw std::runtime_error("unterminated string on line " + std::to_string(line_)); //this will crash the language right now if you dont finish a string
                    }
                    str += advance();
                }
                if(atEnd()){
                    throw std::runtime_error("unterminated string, file ended before closing quote"); //if this is why we exited loop error
                }

                //push token
                Token t;
                t.type = Tok::STRING;
                t.text = str;
                t.line = line_;

                v.push_back(t);
                //consume last input
                advance();
            }
            //handles fields
            else if(c == '$'){

                std::string str;
                //advance to consume the dolla sign
                advance();
                while(isdigit(peek())){
                    str += advance();
                }
                //make sure there is no empty string
                if(str.empty()){
                    throw std::runtime_error("Undesignated field at line " + std::to_string(line_) + ", check for random $"); 
                }

                //create and push vector
                Token t;
                t.type = Tok::FIELD;
                t.text = str; //i dont need this but i want it
                t.num = std::stod(str);
                t.line = line_;

                v.push_back(t);
            }
            //handles numbers
            else if (isdigit(c)) {
                std::string str;

                //whole part
                while (isdigit(peek())) {
                    str += advance();
                }

                //optional decimal point and fractional part
                if (peek() == '.') {
                    str += advance();
                    while (isdigit((unsigned char)peek())) {
                    str += advance();
            }
        }

        Token t;
        t.type = Tok::NUMBER;
        t.text = str;
        t.num = std::stod(str);
        t.line = line_;
        v.push_back(t);
}




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



