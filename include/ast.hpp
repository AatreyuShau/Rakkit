#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <iostream>

enum class Type {
    Int,
    String,
    Void,
    Function
};

//struct Expr {
//    virtual ~Expr() = default;
//};

struct Expr {
    virtual ~Expr() = default;
    virtual void print(int indent) const = 0;
};

struct CallExpr : Expr {
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;

    CallExpr(std::string c, std::vector<std::unique_ptr<Expr>> a)
        : callee(std::move(c)), args(std::move(a)) {}

    void print(int indent) const override {
        printf("%*sCall %s\n", indent, "", callee.c_str());
        for (auto& arg : args)
            arg->print(indent + 2);
    }
};

struct StringExpr : Expr {
    std::string value;
    StringExpr(std::string v) : value(std::move(v)) {}

    void print(int indent) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "StringExpr(\"" << value << "\")\n";
    }
};

struct IntExpr : Expr {
    int value;
    IntExpr(int v) : value(v) {}

    void print(int indent) const override {
        printf("%*sInt %d\n", indent, "", value);
    }
};

struct VarExpr : Expr {
    std::string name;
    VarExpr(std::string n) : name(std::move(n)) {}

    void print(int indent) const override {
        printf("%*sVar %s\n", indent, "", name.c_str());
    }
};

struct BinExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> lhs, rhs;

    BinExpr(std::string o,
            std::unique_ptr<Expr> l,
            std::unique_ptr<Expr> r)
        : op(std::move(o)), lhs(std::move(l)), rhs(std::move(r)) {}

    void print(int indent) const override {
        printf("%*sBin %s\n", indent, "", op.c_str());
        lhs->print(indent + 2);
        rhs->print(indent + 2);
    }
};

//struct Stmt {
//    virtual ~Stmt() = default;
//};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void print(int indent) const = 0;
};

struct LetStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;
    Type type = Type::Void;

    LetStmt(std::string n, std::unique_ptr<Expr> v, Type t)
        : name(std::move(n)), value(std::move(v)), type(t) {}

    void print(int indent) const override {
        printf("%*sLet %s\n", indent, "", name.c_str());
        value->print(indent + 2);
    }
};

struct ExprStmt : Stmt {
    std::unique_ptr<Expr> expr;

    ExprStmt(std::unique_ptr<Expr> e)
        : expr(std::move(e)) {}

    void print(int indent) const override {
        printf("%*sExprStmt\n", indent, "");
        expr->print(indent + 2);
    }
};

struct ReturnStmt : Stmt {
    std::unique_ptr<Expr> value;

    ReturnStmt(std::unique_ptr<Expr> v)
        : value(std::move(v)) {}

    void print(int indent) const override {
        // Print indentation and the "Return" label.
        printf("%*sReturn\n", indent, "");
        if (value) value->print(indent + 2);
    }
};

///

struct Function {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<Stmt>> body;

    void print() const {
        printf("Function %s\n", name.c_str());
        for (auto& stmt : body)
            stmt->print(2);
    }
};

