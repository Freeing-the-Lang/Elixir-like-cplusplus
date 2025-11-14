#include "lexer.hpp"
#include <cctype>

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;

    auto add = [&](TokenType t, const std::string& s=""){
        tokens.push_back({t, s});
    };

    while (i < input.size()) {
        char c = input[i];

        if (isspace(c)) { i++; continue; }

        if (isdigit(c)) {
            std::string num;
            while (i < input.size() && isdigit(input[i]))
                num += input[i++];
            add(TokenType::Number, num);
            continue;
        }

        if (isalpha(c)) {
            std::string id;
            while (i < input.size() && (isalnum(input[i]) || input[i]=='_'))
                id += input[i++];
            add(TokenType::Ident, id);
            continue;
        }

        if (c == '(') { add(TokenType::LParen); i++; continue; }
        if (c == ')') { add(TokenType::RParen); i++; continue; }
        if (c == ',') { add(TokenType::Comma); i++; continue; }

        if (c == '|' && i + 1 < input.size() && input[i+1] == '>') {
            add(TokenType::Pipe, "|>");
            i += 2;
            continue;
        }

        i++; // skip unknown
    }

    add(TokenType::EOFToken);
    return tokens;
}
