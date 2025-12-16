#pragma once
#include <string>
#include <vector>
#include "token.hpp"

class Lexer {
public:
    Lexer(const std::string& source);

    std::vector<Token> tokenize();

private:
    char peek() const;
    char advance();
    bool isAtEnd() const;

    void skipWhitespace();
    Token identifier();
    Token number();

    std::string src;
    size_t pos = 0;
};
