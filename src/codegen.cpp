#include "codegen.hpp"

std::string genExpr(ExprPtr e);

std::string genPipe(PipeExpr* p) {
    std::string cur = genExpr(p->chain[0]);

    for (size_t i = 1; i < p->chain.size(); i++) {
        auto call = dynamic_cast<CallExpr*>(p->chain[i].get());
        if (!call) continue;

        std::string temp = call->func + "(" + cur;

        for (auto& a : call->args)
            temp += ", " + genExpr(a);

        temp += ")";
        cur = temp;
    }

    return cur;
}

std::string genExpr(ExprPtr e) {
    if (auto n = dynamic_cast<NumberExpr*>(e.get()))
        return std::to_string(n->value);

    if (auto id = dynamic_cast<IdentExpr*>(e.get()))
        return id->name;

    if (auto c = dynamic_cast<CallExpr*>(e.get())) {
        std::string out = c->func + "(";
        for (size_t i=0; i < c->args.size(); i++) {
            out += genExpr(c->args[i]);
            if (i + 1 < c->args.size()) out += ", ";
        }
        return out + ")";
    }

    if (auto p = dynamic_cast<PipeExpr*>(e.get()))
        return genPipe(p);

    return "";
}

std::string generateCpp(ExprPtr expr) {
    return "int result = " + genExpr(expr) + ";";
}
