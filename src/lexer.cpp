#include "lexer.hpp"
#include <cctype>

Lexer::Lexer(const std::string& source) : src(source) {}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    return src[pos++];
}

bool Lexer::isAtEnd() const {
    return pos >= src.size();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(peek())) {
        advance();
    }
}

Token Lexer::identifier() {
    size_t start = pos;
    while (!isAtEnd() && std::isalnum(peek())) {
        advance();
    }

    std::string text = src.substr(start, pos - start);

    if (text == "fn")     return {TokenType::Fn, text};
    if (text == "let")    return {TokenType::Let, text};
    if (text == "return") return {TokenType::Return, text};

    return {TokenType::Ident, text};
}

Token Lexer::number() {
    size_t start = pos;
    while (!isAtEnd() && std::isdigit(peek())) {
        advance();
    }

    return {TokenType::Number, src.substr(start, pos - start)};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;

        char c = advance();

        if (c == '"') {
            std::string value;

            while (!isAtEnd() && peek() != '"') {
                value += advance();
            }

            if (isAtEnd())
                throw std::runtime_error("Unterminated string literal");

            advance(); // consume closing "
            tokens.push_back({TokenType::String, value});
            continue;
        }

        switch (c) {
            case '(': tokens.push_back({TokenType::LParen, "("}); break;
            case ')': tokens.push_back({TokenType::RParen, ")"}); break;
            case ']': tokens.push_back({TokenType::RSquare, "]"}); break;
            case '{': tokens.push_back({TokenType::LBrace, "{"}); break;
            case '}': tokens.push_back({TokenType::RBrace, "}"}); break;
            case '+': tokens.push_back({TokenType::Plus, "+"}); break;
            case '-': tokens.push_back({TokenType::Minus, "-"}); break;
            case '*': tokens.push_back({TokenType::Star, "*"}); break;
            case '/': tokens.push_back({TokenType::Slash, "/"}); break;
            case ',': tokens.push_back({TokenType::Comma, ","}); break;

            case ':':
                tokens.push_back({TokenType::ColonEq, ":="});
                break;

            default:
                if (std::isalpha(c)) {
                    pos--;  // unread
                    tokens.push_back(identifier());
                } else if (std::isdigit(c)) {
                    pos--;
                    tokens.push_back(number());
                }
                break;
        }
    }

    tokens.push_back({TokenType::EOFToken, ""});
    return tokens;
}
