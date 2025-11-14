#pragma once
#include <string>
#include <vector>
#include <memory>

struct Expr {
    virtual ~Expr() = default;
};

using ExprPtr = std::shared_ptr<Expr>;

struct NumberExpr : Expr {
    int value;
    NumberExpr(int v) : value(v) {}
};

struct IdentExpr : Expr {
    std::string name;
    IdentExpr(const std::string& n) : name(n) {}
};

struct CallExpr : Expr {
    std::string func;
    std::vector<ExprPtr> args;

    CallExpr(const std::string& f, const std::vector<ExprPtr>& a)
        : func(f), args(a) {}
};

struct PipeExpr : Expr {
    std::vector<ExprPtr> chain;
    PipeExpr(const std::vector<ExprPtr>& c) : chain(c) {}
};
