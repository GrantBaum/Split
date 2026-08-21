//include statements
#include "lexer/lexer.h"
#include <fstream>
#include <sstream>
#include <iostream>

//main method
int main(int argc, char* argv[]){

    //argument check
    if(argc < 2) {
        std::cerr << "Not enough arguments!" << std::endl; //make good error messages. There will be lots.
        return 1;
    }
    //valid number of args, lets continue
    //the name of the script to execute for the ifstream
    std::string scriptFile = argv[1];
    std::ifstream inFile(scriptFile);

    //check for successful opening
    if(!inFile) {
        std::cerr << "Failed to open split file" << std::endl; //didnt work
        return 1;
    }

    //opened successfully start the stringstream and get full code
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string source = buffer.str();

    //now that we have the full string we can lex
    split::Lexer L(source);
    std::vector<split::Token> tokens = L.lex(); //this var should now hold our vector of tokens
    
    //loop the tokens and print their type and whats in the text instance var
    for (const auto& t : tokens){
        std::cout << split::tokName(t.type) + " [" + t.text + "] " << std::endl;
    }
    return 0;

}