#pragma once
#include <vector>
#include <memory>
#include "token.hpp"
#include "ast.hpp"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    Function parseFunction();
    bool isAtEnd() const;

private:
    const Token& peek() const;
    const Token& advance();
    bool match(TokenType type);
    void expect(TokenType type, const char* msg);

    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();

    const std::vector<Token>& tokens;
    size_t pos = 0;
};
