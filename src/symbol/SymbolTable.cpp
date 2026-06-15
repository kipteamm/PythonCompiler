#include "SymbolTable.h"


SymbolTable::SymbolTable(const SymbolTable *parent) : parent(parent), id(tableId++) {}


SymbolTable* SymbolTable::newScope() {
    auto table = std::make_unique<SymbolTable>(this);
    SymbolTable* rawPtr = table.get();

    this->children.emplace(std::move(table));

    return rawPtr;
}


void SymbolTable::addSymbol(const std::string &identifier, TOKENTYPE type) {
    symbols[identifier] = std::make_unique<Symbol>(type);
}


Symbol *SymbolTable::getSymbol(const std::string &identifier) const {
    const auto it = symbols.find(identifier);

    if (it == symbols.end()) return nullptr;
    return it->second.get();
}


std::string SymbolTable::getDot() {
    // TABLE HEADER
    oss << "\t" << *this << "\n";
    oss << "\t[label=<\n";
    oss << "\t<TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACE=\"0\">\n";
    oss << "\t\t<TR>\n";
    oss << "\t\t\t<TD><B>Identifier</B></TD>\n";
    oss << "\t\t\t<TD><B>Type</B></TD>\n";
    oss << "\t\t</TR>\n";

    // TABLE ENTIRES
    for (const auto& [identifier, symbol] : symbols) {
        oss << "\t\t<TR>\n";
        oss << "\t\t\t<TD>" << identifier << "</TD>";
        oss << "\t\t\t<TD>" << tokenTypeToString(symbol->type) << "</TD>";
        oss << "\t\t</TR>";
    }

    oss << "\t</TABLE>\n";
    oss << "\t>];\n";

    // CHILDREN
    for (const auto& table: children) {
        oss << table->getDot();
        oss << "\t" << *this << " -> " << *table;
    }

    return oss.str();
}
