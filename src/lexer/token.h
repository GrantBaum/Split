//include statements
#pragma once
#include <string>

namespace split {
    //starting with my enum
    enum class Tok {
        IDENT, NUMBER, STRING, FIELD, //things like price, 100, "hi", $2 (respectively)
        KEYWORD, //where, read, print
        OP, //> < = + - * / , { } ( ), operators
        INVALID, //a value for unset or invalid so theres no leftover c garbage;
        NEWLINE, END
    };

    struct Token {
        Tok type = Tok::INVALID;
        std::string text;
        double num = 0; //for NUMBER and FIELD
        int line = 1; //for error messages
    };

    //inline says to the linker that this will always be the same and it can just pick whatever one it wants
    inline std::string tokName(Tok t) {
    // switch on t, return a string for each case
    switch(t){
        //literally just converts token type into a string hardcoded
        case Tok::NUMBER:
            return "NUMBER";
        case Tok::IDENT:
            return "IDENT";
        case Tok::STRING:
            return "STRING";
        case Tok::FIELD:
            return "FIELD";
        case Tok::KEYWORD:
            return "KEYWORD";
        case Tok::OP:
            return "OPERATOR";
        case Tok::NEWLINE:
            return "NEWLINE";
        case Tok::END:
            return "END";
        case Tok::INVALID:
            return "INVALID";
    }
    //in case someone casts a value that i didnt put in my list
    return "?";
}
}

