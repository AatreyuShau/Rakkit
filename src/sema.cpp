#include "sema.hpp"
#include <iostream>
#include <typeinfo>
#include <set>

static std::set<std::string> builtins = {
    "input",
    "print",
    "csvRead",
    "csvGet"
};

static std::set<std::string> functions;

void SymbolTable::enterScope() {
    scopes.push_back({});
}

void SymbolTable::exitScope() {
    scopes.pop_back();
}

bool SymbolTable::declare(const std::string& name, Type type) {
    if (scopes.back().count(name))  {
        return false;
    }
    scopes.back()[name] = type;
    return true;
}

Type SymbolTable::lookup(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto f = it->find(name);
        if (f != it->end())
            return f->second;
    }

    if (name == "input")  return Type::Int;
    if (name == "print")  return Type::Void;
    if (name == "csvRead")  return Type::CSV;
    if (name == "csvGet")  return Type::String;

    if (functions.count(name))
        return Type::Function;

    throw std::runtime_error("Undefined identifier: " + name);
}

Type analyzeExpr(const Expr* expr, SymbolTable& table) {
    if (dynamic_cast<const IntExpr*>(expr)) {
        return Type::Int;
    }
    else if (dynamic_cast<const StringExpr*>(expr)) {
        return Type::String;
    }
    if (auto varExpr = dynamic_cast<const VarExpr*>(expr)) {
        return table.lookup(varExpr->name);
    }
    else if (auto bin = dynamic_cast<const BinExpr*>(expr)) {
        Type l = analyzeExpr(bin->lhs.get(), table);
        Type r = analyzeExpr(bin->rhs.get(), table);

        if (l != Type::Int || r != Type::Int)
            throw std::runtime_error("Binary operators only allowed on ints");

        return Type::Int;
    }
    else if (auto callExpr = dynamic_cast<const CallExpr*>(expr)) {
        Type calleeType = table.lookup(callExpr->callee);

        if (calleeType != Type::Function &&
            callExpr->callee != "input" &&
            callExpr->callee != "print" &&
            callExpr->callee != "csvRead" &&
            callExpr->callee != "csvGet") {
            throw std::runtime_error("Not a function: " + callExpr->callee);
        }

        for (auto& arg : callExpr->args)
            analyzeExpr(arg.get(), table);

        if (callExpr->callee == "print") return Type::Void;
        if (callExpr->callee == "input") return Type::Int;
        if (callExpr->callee == "csvRead") return Type::CSV;
        if (callExpr->callee == "csvGet") return Type::String;

        return Type::Int;
    }

    throw std::runtime_error("Unknown expression");
}

void analyzeStmt(Stmt* stmt, SymbolTable& table) {
    if (auto letStmt = dynamic_cast<LetStmt*>(stmt)) {
        Type rhs = analyzeExpr(letStmt->value.get(), table);
        letStmt->type = rhs;
        if (!table.declare(letStmt->name, rhs)) {
            throw std::runtime_error("Variable already declared: " + letStmt->name);
        }
    }
    else if (auto exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
        analyzeExpr(exprStmt->expr.get(), table);
    }
}

void analyzeFunction(const Function& fn) {
    SymbolTable table;
    table.enterScope();

    for (auto& p : fn.params) { 
        table.declare(p, Type::Int);
    }
    for (auto& stmt : fn.body) {
        analyzeStmt(stmt.get(), table);
    }

    table.exitScope();
}

void analyzeProgram(const std::vector<Function>& funcs) {
    functions.clear();

    for (auto& b : builtins)
        functions.insert(b);
    for (auto& fn : funcs) {
        if (functions.count(fn.name)) {
            throw std::runtime_error("Duplicate function: " + fn.name);
        }
        functions.insert(fn.name);
    }

    for (auto& fn : funcs) {
        analyzeFunction(fn);
    }
}