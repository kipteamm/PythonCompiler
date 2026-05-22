#include <fstream>
#include <iostream>

#include "src/lexer/Lexer.h"
#include "src/parsing/Parser.h"


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

    Parser parser(tokens);
    auto scope = parser.start();

    return 0;
}
