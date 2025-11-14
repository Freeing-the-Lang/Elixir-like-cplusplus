#include "parser.hpp"

Parser::Parser(const std::vector<Token>& t)
    : tokens(t), pos(0) {}

Token Parser::peek() { return tokens[pos]; }
Token Parser::advance() { return tokens[pos++]; }

ExprPtr Parser::parse() {
    return parsePipe();
}

ExprPtr Parser::parsePipe() {
    std::vector<ExprPtr> chain;

    chain.push_back(parseExpr());

    while (peek().type == TokenType::Pipe) {
        advance();
        chain.push_back(parseExpr());
    }

    if (chain.size() == 1) return chain[0];
    return std::make_shared<PipeExpr>(chain);
}

ExprPtr Parser::parseExpr() {
    ExprPtr left = parsePrimary();
    return parseCall(left);
}

ExprPtr Parser::parsePrimary() {
    Token t = advance();

    if (t.type == TokenType::Number)
        return std::make_shared<NumberExpr>(std::stoi(t.text));

    if (t.type == TokenType::Ident)
        return std::make_shared<IdentExpr>(t.text);

    return nullptr;
}

ExprPtr Parser::parseCall(ExprPtr base) {
    if (peek().type != TokenType::LParen)
        return base;

    advance(); // '('

    std::vector<ExprPtr> args;

    if (peek().type != TokenType::RParen) {
        args.push_back(parseExpr());
        while (peek().type == TokenType::Comma) {
            advance();
            args.push_back(parseExpr());
        }
    }

    if (peek().type == TokenType::RParen)
        advance();

    auto id = dynamic_cast<IdentExpr*>(base.get());

    return std::make_shared<CallExpr>(id->name, args);
}
