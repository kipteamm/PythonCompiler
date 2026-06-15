#include <fstream>
#include <iostream>

#include "src/ast/ASTRenderer.h"
#include "src/lexer/Lexer.h"
#include "src/parser/Parser.h"


void debugTable(SymbolTable& table) {
    std::ofstream outFile("output/symbol.dot");

    outFile << "digraph symbol {\n";
    outFile << "\tsplines=true; nodesep=0.5; ranksep=0.5\n\n";
    outFile << "\tnode [shape=box, fontname=\"Courier\"]\n\n";
    outFile << table.getDot();
    outFile << "\n}";

    outFile.close();
}


void debugAST(Scope& scope) {
    ASTRenderer dotPrinter;
    scope.accept(&dotPrinter);

    std::ofstream outFile("output/ast.dot");
    outFile << dotPrinter.getDot();
    outFile.close();
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1;
    }

    const std::string filePath = argv[1];
    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "Error: Could not open file '" << filePath << "'\n";
        return 1;
    }

    const std::string fileContent(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    Lexer lexer(fileContent);
    const std::vector<Token> tokens = lexer.scan();

    for (const Token& token : tokens) {
        std::cout << token << ", ";
    }
    std::cout << std::endl;

    auto table = std::make_unique<SymbolTable>();

    Parser parser(tokens);
    auto scope = parser.start();
    
    debugTable(*table);
    debugAST(*scope);

    return 0;
}
