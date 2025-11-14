#include "parser.hpp"
#include <iostream>

// ------------------------------------
// 기본 설정
// ------------------------------------
Parser::Parser(const std::vector<Token>& t)
    : tokens(t), pos(0) {}

Token Parser::peek() {
    if (pos >= tokens.size())
        return Token{TokenType::EOFToken, ""};
    return tokens[pos];
}

Token Parser::advance() {
    if (pos >= tokens.size())
        return Token{TokenType::EOFToken, ""};
    return tokens[pos++];
}

// ------------------------------------
// 전체 파싱 시작
// ------------------------------------
ExprPtr Parser::parse() {
    return parsePipe();
}

// ------------------------------------
// PipeExpr:  a |> b(x) |> c()
// ------------------------------------
ExprPtr Parser::parsePipe() {
    std::vector<ExprPtr> chain;

    chain.push_back(parseExpr());

    while (peek().type == TokenType::Pipe) {
        advance(); // consume |>

        if (peek().type == TokenType::EOFToken)
            break;

        chain.push_back(parseExpr());
    }

    if (chain.size() == 1)
        return chain[0];

    return std::make_shared<PipeExpr>(chain);
}

// ------------------------------------
// 일반 Expression
// ------------------------------------
ExprPtr Parser::parseExpr() {
    ExprPtr left = parsePrimary();
    return parseCall(left);
}

// ------------------------------------
// Primary: Number, Ident
// ------------------------------------
ExprPtr Parser::parsePrimary() {
    Token t = advance();

    if (t.type == TokenType::Number)
        return std::make_shared<NumberExpr>(std::stoi(t.text));

    if (t.type == TokenType::Ident)
        return std::make_shared<IdentExpr>(t.text);

    return nullptr;
}

// ------------------------------------
// CallExpr:  func(a,b)
// ------------------------------------
ExprPtr Parser::parseCall(ExprPtr base) {
    // EOF 방지
    if (peek().type == TokenType::EOFToken)
        return base;

    // '(' 없으면 함수 호출 아님
    if (peek().type != TokenType::LParen)
        return base;

    // base 가 identifier 여야 함
    auto id = dynamic_cast<IdentExpr*>(base.get());
    if (!id) {
        // 잘못된 구문이라도 파서가 죽지 않도록 보호
        return base;
    }

    advance(); // '(' consume

    std::vector<ExprPtr> args;

    // EOF 방지
    if (peek().type == TokenType::EOFToken)
        return base;

    // 파라미터가 있다면
    if (peek().type != TokenType::RParen) {
        args.push_back(parseExpr());

        while (peek().type == TokenType::Comma) {
            advance(); // ','
            args.push_back(parseExpr());
        }
    }

    // ')' consume
    if (peek().type == TokenType::RParen)
        advance();

    return std::make_shared<CallExpr>(id->name, args);
}
