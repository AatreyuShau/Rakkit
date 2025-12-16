#include "codegen.hpp"
#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, Type> varTypes;

static void genExpr(std::ofstream& out, const Expr* expr) {
    if (auto intExpr = dynamic_cast<const IntExpr*>(expr)) {
        out << intExpr->value;
    }
    else if (auto strExpr = dynamic_cast<const StringExpr*>(expr)) {
        out << "\"" << strExpr->value << "\"";
    }
    else if (auto varExpr = dynamic_cast<const VarExpr*>(expr)) {
        out << varExpr->name;
    }
    else if (auto binExpr = dynamic_cast<const BinExpr*>(expr)) {
        out << "(";
        genExpr(out, binExpr->lhs.get());
        out << " " << binExpr->op << " ";
        genExpr(out, binExpr->rhs.get());
        out << ")";
    }
    else if (auto callExpr = dynamic_cast<const CallExpr*>(expr)) {
        if (callExpr->callee == "input") {
            out << "({ int __tmp; scanf(\"%d\", &__tmp); __tmp; })";
            return;
        } 
        else if (callExpr->callee == "print") {
            out << "printf(";

            const Expr* arg = callExpr->args[0].get();

            bool isString = false;

            if (dynamic_cast<const StringExpr*>(arg)) {
                isString = true;
            }
            else if (auto v = dynamic_cast<const VarExpr*>(arg)) {
                if (varTypes[v->name] == Type::String)
                    isString = true;
            }

            out << (isString ? "\"%s\\n\", " : "\"%d\\n\", ");
            
            //genExpr(out, callExpr->args[0].get());
            for (size_t i = 0; i < callExpr->args.size(); ++i) {
                genExpr(out, callExpr->args[i].get());
                if (i + 1 < callExpr->args.size()) out << ", ";
            }
            out << ")";
        } 
        else {
            out << callExpr->callee << "(";
            for (size_t i = 0; i < callExpr->args.size(); ++i) {
                genExpr(out, callExpr->args[i].get());
                if (i + 1 < callExpr->args.size()) out << ", ";
            }
            out << ")";
        }
    }
}

static void genStmt(std::ofstream& out, const Stmt* stmt) {
    if (auto letStmt = dynamic_cast<const LetStmt*>(stmt)) {
        varTypes[letStmt->name] = letStmt->type;

        if (letStmt->type == Type::Int)
            out << "int ";
        else if (letStmt->type == Type::String)
            out << "const char* ";
        out << letStmt->name << " = ";
        genExpr(out, letStmt->value.get());
        out << ";\n";
    }
    else if (auto exprStmt = dynamic_cast<const ExprStmt*>(stmt)) {
        genExpr(out, exprStmt->expr.get());
        out << ";\n";
    }
}

void genFn(std::ofstream& out, const Function& fn) {
    out << "int " << fn.name << "(";
    for (size_t i = 0; i < fn.params.size(); ++i) {
        out << "int " << fn.params[i];
        if (i + 1 < fn.params.size()) out << ", ";
    }
    out << ") {\n";

    for (auto& stmt : fn.body) {
        if (auto ret = dynamic_cast<const ReturnStmt*>(stmt.get())) {
            out << "return ";
            genExpr(out, ret->value.get());
            out << ";\n";
        } else {
            genStmt(out, stmt.get());
        }
    }

    out << "}\n";
}

void generateC(const std::vector<Function>& functions, const std::string& filename) {

    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Cannot open " << filename << "\n";
        return;
    }

    out << "#include <stdio.h>\n\n";

    // Generate all functions
    for (const auto& fn : functions) {
        out << "int " << fn.name << "(";

        for (size_t i = 0; i < fn.params.size(); ++i) {
            out << "int " << fn.params[i];
            if (i + 1 < fn.params.size()) out << ", ";
        }

        out << ") {\n";

        for (const auto& stmt : fn.body) {
            if (auto ret = dynamic_cast<ReturnStmt*>(stmt.get())) {
                out << "return ";
                genExpr(out, ret->value.get());
                out << ";\n";
            } else {
                genStmt(out, stmt.get());
            }
        }

        out << "}\n\n";
    }

    out.close();
    std::cout << "Generated " << filename << "\n";
}