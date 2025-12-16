#pragma once
#include "ast.hpp"
#include <memory>
#include <fstream>
#include <string>
#include <vector>

void generateC(const std::vector<Function>& functions, const std::string& filename = "output.c");
