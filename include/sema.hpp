#pragma once
#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

struct SymbolTable {
    std::vector<std::unordered_map<std::string, Type>> scopes;

    void enterScope();
    void exitScope();
    bool declare(const std::string& name, Type type);
    Type lookup(const std::string& name);
};

// Semantic analysis functions
void analyzeStmt(const Stmt* stmt, SymbolTable& table);
Type analyzeExpr(const Expr* expr, SymbolTable& table);
void analyzeFunction(const Function& fn);
void analyzeProgram(const std::vector<Function>& functions);