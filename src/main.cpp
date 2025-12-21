#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <dirent.h>

#include "lexer.hpp"
#include "parser.hpp"
#include "sema.hpp"
#include "codegen.hpp"

void peepPath(const std::string& path) {
    struct stat s;
    if (stat(path.c_str(), &s) != 0) {
        std::cerr << "rak peep: path does not exist: " << path << "\n";
        return;
    }

    if (S_ISREG(s.st_mode)) {
        std::ifstream in(path);
        std::cout << "~~~~~~| " << path << " |~~~~~~\n";
        std::cout << in.rdbuf() << "\n";
        return;
    }

    if (S_ISDIR(s.st_mode)) {
        DIR* dir = opendir(path.c_str());
        if (!dir) {
            std::cerr << "rak peep: cannot open directory: " << path << "\n";
            return;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string name = entry->d_name;
            if (name == "." || name == "..") continue;

            std::string fullPath = path + "/" + name;

            struct stat entryStat;
            if (stat(fullPath.c_str(), &entryStat) != 0) continue;

            if (S_ISREG(entryStat.st_mode)) {
                std::ifstream in(fullPath);
                std::cout << "~~~~~~| " << fullPath << " |~~~~~~\n";
                std::cout << in.rdbuf() << "\n";
            }
            // You can recursively call peepPath(fullPath) if you want recursion
        }
        closedir(dir);
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage:\n";
        std::cerr << "  rak race <file.rk>\n";
        std::cerr << "  rak peep <file|dir>\n";
        return 1;
    }

    std::string command = argv[1];
    std::string filename = argv[2];

    if (command == "peep") {
        peepPath(filename); 
        return 0;
    }

    if (command != "race") {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }

    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    Lexer lexer(buffer.str());
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    std::vector<Function> functions;

    while (!parser.isAtEnd()) {
        functions.push_back(parser.parseFunction());
    }

    try {
        analyzeProgram(functions);
        std::cout << "Semantic analysis passed!\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Semantic error: " << e.what() << "\n";
        return 1;
    }

    generateC(functions, "output.c");

    std::cout << "[clang] compiling output.c...\n";
    if (system("clang output.c runtime/csv.c -o output") != 0) {
        std::cerr << "C compilation failed\n";
        return 1;
    }

    std::cout << "[run]\n";
    system("./output");

    return 0;

}