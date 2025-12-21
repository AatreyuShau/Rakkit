#pragma once
#include <string>

enum class TokenType {
    //keywords
    Fn,
    Let,
    Return,

    //idents & literals
    Ident,
    Number,

    //symbol type ish
    LParen, RParen,
    RSquare,
    LBrace, RBrace,
    Plus, Minus, Star, Slash,
    ColonEq,

    Comma,

    EOFToken,

    //dataTypes
    String,
    Int,
    CSV,
    Row
};

struct Token {
    TokenType type;
    std::string lexeme;
};
