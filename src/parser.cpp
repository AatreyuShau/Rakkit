#include "parser.hpp"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& t) : tokens(t) {}

const Token& Parser::peek() const {
    return tokens[pos];
}

const Token& Parser::advance() {
    return tokens[pos++];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EOFToken;
}

void Parser::expect(TokenType type, const char* msg) {
    if (peek().type != type)
        throw std::runtime_error(msg);
    advance();
}

Function Parser::parseFunction() {
    expect(TokenType::Fn, "Expected 'fn'");
    std::string name = advance().lexeme; // ident

    expect(TokenType::LParen, "Expected '(' after function name");
    std::vector<std::string> params;

    if (!match(TokenType::RParen)) {
        do {
            params.push_back(advance().lexeme); // ident
        } while (match(TokenType::Comma));
        if (peek().type != TokenType::RParen) {
            throw std::runtime_error("Expected ')' 1");
        }
        advance();
    }

    expect(TokenType::LBrace, "Expected '{' before function body");

    Function fn;
    fn.name = name;
    fn.params = std::move(params);

    while (peek().type != TokenType::RBrace) {
        fn.body.push_back(parseStatement());
    }

    expect(TokenType::RBrace, "Expected '}' after function body");

    return fn;
}

std::unique_ptr<Stmt> Parser::parseStatement() {
    if (match(TokenType::Let)) {
        std::string name = advance().lexeme; // ident
        advance(); // :=
        auto value = parseExpression();
        Type t;
        if (dynamic_cast<StringExpr*>(value.get()))
            t = Type::String;
        else
            t = Type::Int;
        return std::make_unique<LetStmt>(name, std::move(value), t);
    }

    if (match(TokenType::Return)) {
        auto val = parseExpression();
        return std::make_unique<ReturnStmt>(std::move(val));
    }

    auto expr = parseExpression();
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::parseExpression() {
    auto left = parseTerm();

    while (peek().type == TokenType::Plus ||
           peek().type == TokenType::Minus) {
        std::string op(1, advance().lexeme[0]);
        auto right = parseTerm();
        left = std::make_unique<BinExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseTerm() {
    auto left = parseFactor();

    while (peek().type == TokenType::Star ||
           peek().type == TokenType::Slash) {
        std::string op(1, advance().lexeme[0]);
        auto right = parseFactor();
        left = std::make_unique<BinExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseFactor() {
    if (match(TokenType::String)) {
        return std::make_unique<StringExpr>(tokens[pos - 1].lexeme);
    }

    if (match(TokenType::Number)) {
        return std::make_unique<IntExpr>(std::stoi(tokens[pos - 1].lexeme));
    }

    if (match(TokenType::Ident)) {
        std::string name = tokens[pos - 1].lexeme;

        if (match(TokenType::LParen)) {
            std::vector<std::unique_ptr<Expr>> args;

            auto ogPos = pos;

            if (!match(TokenType::RParen)) {
                do {
                    args.push_back(parseExpression());
                } while (match(TokenType::Comma));

                if (!match(TokenType::RParen)) {
                    pos = ogPos;
                    if (!match(TokenType::RSquare)) {
                        do {
                            args.push_back(parseExpression());
                        } while (match(TokenType::Comma));

                        if (!match(TokenType::RSquare)) {
                            throw std::runtime_error("Expected ')' 2");
                        } else {
                            return std::make_unique<CallExpr>("printin", std::move(args));
                        }
                    } //
                }
            }

            return std::make_unique<CallExpr>(name, std::move(args));
        }

        return std::make_unique<VarExpr>(name);
    }

    if (match(TokenType::LParen)) {
        auto expr = parseExpression();
        expect(TokenType::RParen, "Expected ')' 3");
        return expr;
    }

    throw std::runtime_error("Unexpected token");
}
