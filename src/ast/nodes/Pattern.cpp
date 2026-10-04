#include "Pattern.h"

#include <utility>


CapturePattern::CapturePattern(Token  identifier)
    : identifier(std::move(identifier)) {}


LiteralPattern::LiteralPattern(std::unique_ptr<Literal> literal)
    : literal(std::move(literal)) {}


OrPattern::OrPattern(std::vector<std::unique_ptr<Pattern>> options)
    : options(std::move(options)) {}


SequencePattern::SequencePattern(std::vector<std::unique_ptr<Pattern>> elements)
    : elements(std::move(elements)) {}
