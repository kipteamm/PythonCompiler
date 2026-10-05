#include "Parser.h"


std::unique_ptr<Pattern> Parser::casePattern() {
    auto pat = orPattern();

    // Check for open sequence: case 1, 2:
    if (!match(COMMA))
        return pat;

    std::vector<std::unique_ptr<Pattern>> elements;
    elements.push_back(std::move(pat));

    // As long as were not at the end of the file or case OR at the start of
    // the guard clause.
    while (peek().type != COLON && peek().type != IF && peek().type != END) {
        elements.push_back(orPattern());
        if (!match(COMMA)) break;
    }

    return std::make_unique<SequencePattern>(std::move(elements));
}


std::unique_ptr<Pattern> Parser::orPattern() {
    auto first = primaryPattern();

    if (peek().type != PIPE)
        return first;

    std::vector<std::unique_ptr<Pattern>> alternatives;
    alternatives.push_back(std::move(first));

    while (match(PIPE))
        alternatives.push_back(primaryPattern());

    return std::make_unique<OrPattern>(std::move(alternatives));
}


std::unique_ptr<Pattern> Parser::primaryPattern() {
    // Parenthesized sequence (x, ...):
    if (match(LEFT_PAREN))
        return sequencePattern(RIGHT_PAREN);

    // Bracketed sequence [x, ...]:
    if (match(LEFT_BRACKET))
        return sequencePattern(RIGHT_BRACKET);

    if (peek().type == IDENTIFIER) {
        const auto identifier = advance();
        if (identifier.lexeme == "_")
            return std::make_unique<WildcardPattern>();

        return std::make_unique<CapturePattern>(identifier);
    }

    auto expr = literal();
    return std::make_unique<LiteralPattern>(std::move(expr));
}


std::unique_ptr<SequencePattern> Parser::sequencePattern(TOKENTYPE closingToken) {
    std::vector<std::unique_ptr<Pattern>> elements;

    // Handle empty sequence () or []
    if (match(closingToken))
        return std::make_unique<SequencePattern>(std::move(elements));

    bool hasStar = false;
    while (true) {
        // THE only StarPattern of this sequence
        if (match(STAR)) {
            if (hasStar) throw new std::runtime_error("multiple starred names in sequence pattern");

            hasStar = true;

            const Token id = consume(IDENTIFIER, "expected identifier or '_' after '*'");
            std::unique_ptr<Pattern> target;

            if (id.lexeme == "_")
                target = std::make_unique<WildcardPattern>();
            else
                target = std::make_unique<CapturePattern>(id);

            elements.push_back(std::make_unique<StarPattern>(std::move(target)));
        } else {
            // Elements of a sequence can themselves be or-patterns: case (1 | 2, 3):
            elements.push_back(orPattern());
        }

        if (match(closingToken)) break;
        consume(COMMA, "expected ',' or closing delimiter");

        // Trailing comma case (1,):
        if (match(closingToken)) break;
    }

    return std::make_unique<SequencePattern>(std::move(elements));
}
