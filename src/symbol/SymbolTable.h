#ifndef PYTHONCOMPILER_SYMBOLTABLE_H
#define PYTHONCOMPILER_SYMBOLTABLE_H

#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <memory>
#include <string>

#include "../common/Token.h"


struct Symbol {
    // might want to change this to a different enum in the future and some
    // helper functions isntead, because currently the type of a symbol can
    // also be "if" or "while" (you get the idea, time will tell)
    TOKENTYPE type;
};


class SymbolTable {
public:
    SymbolTable() : id(tableId++) {};
    explicit SymbolTable(const SymbolTable* parent);

    [[nodiscard]] SymbolTable* newScope();

    void addSymbol(const std::string& identifier, TOKENTYPE type);
    [[nodiscard]] Symbol* getSymbol(const std::string& identifier) const;

    std::string getDot();

    friend std::ostream& operator<<(std::ostream& stream, const SymbolTable& node) {
        return stream << "table_" << node.id;
    }

private:
    const SymbolTable* parent = nullptr;
    int id;

    std::unordered_set<std::unique_ptr<SymbolTable>> children = {};
    std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols = {};

    std::ostringstream oss;

    inline static int tableId;
};


#endif //PYTHONCOMPILER_SYMBOLTABLE_H
