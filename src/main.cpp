#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

int main() {
    std::ifstream f("example.elx");
    std::stringstream ss;
    ss << f.rdbuf();

    std::string src = ss.str();

    auto tokens = tokenize(src);
    Parser p(tokens);
    ExprPtr ast = p.parse();

    std::string cpp = generateCpp(ast);

    std::cout << "Generated C++:\n" << cpp << "\n";
}
