#pragma once
#include "lexer.hpp"
#include "ast.hpp"

class Parser {
public:
    Parser(const std::vector<Token>& t);

    ExprPtr parse();

private:
    const std::vector<Token>& tokens;
    int pos;

    Token peek();
    Token advance();

    ExprPtr parsePipe();
    ExprPtr parseExpr();
    ExprPtr parsePrimary();
    ExprPtr parseCall(ExprPtr base);
};
