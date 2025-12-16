#pragma once

#include <vector>
#include "token.hpp"
#include "ast.hpp"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    Function parseFunction();
    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Expr> parseExpression();

    bool isAtEnd() const;

private:
    const std::vector<Token>& tokens;
    size_t pos = 0;

    const Token& peek() const;
    const Token& advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
};