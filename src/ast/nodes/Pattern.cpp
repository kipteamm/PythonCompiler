#include "Pattern.h"

#include <utility>


CapturePattern::CapturePattern(Token  identifier)
    : identifier(std::move(identifier)) {}


DictionaryPattern::DictionaryPattern(std::vector<std::pair<std::unique_ptr<Literal>, std::unique_ptr<Pattern>>> entries, std::unique_ptr<Pattern> rest)
    : entries(std::move(entries)), rest(std::move(rest)) {}

LiteralPattern::LiteralPattern(std::unique_ptr<Literal> literal)
    : literal(std::move(literal)) {}


OrPattern::OrPattern(std::vector<std::unique_ptr<Pattern>> options)
    : options(std::move(options)) {}


StarPattern::StarPattern(std::unique_ptr<Pattern> pattern)
    : pattern(std::move(pattern)) {}


SequencePattern::SequencePattern(std::vector<std::unique_ptr<Pattern>> elements)
    : elements(std::move(elements)) {}
