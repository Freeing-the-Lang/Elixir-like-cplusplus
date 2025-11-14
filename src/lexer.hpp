#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Number,
    Ident,
    Pipe,      // |>
    LParen,
    RParen,
    Comma,
    EOFToken
};

struct Token {
    TokenType type;
    std::string text;
};

std::vector<Token> tokenize(const std::string& input);
