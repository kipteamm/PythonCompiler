/*
 * THIS GRAMMAR IS A REPRESENTATION OF WHAT THE PARSER CLASS DOES, IT IS NOT
 * ACTAULLY USED TO GENERATE THE PARSER CLASS. THAT IS ALSO THE REASON SOME
 * ANTLR4 SYNTAX ERRORS CAN BE FOUND (CUZ I CAN'T BE BOTHERED).
 * SIMIRLY THERE IS NO MENTION OF INDENTS AND DEDENTS, WHILE THIS IS IMPORTANT
 * FOR A PYTHON GRAMMAR TO BE CORRECT, I REALLY CAN'T BE BOTHERED :)
**/

start: statement* EOF;


statement
    : assignment
    | COMMENT
    | for
    | function
    | if_statement
    | match
    | while
    | return;   // Parser does have a check for whether this is inside a function

type_params: '[' single_type (',' single_type)* ']';
single_type: IDENTIFIER (':' types)?;
types: TYPE (type_params)? ('|' types)*;

assignment: IDENTIFIER (':' types)? (('=' expression)? | compound_assignment expression);
compound_assignment: '+=' | '-=' | '/=' | '*=' | '**=' | '//=' | '%='

function: 'def' IDENTIFIER type_params? '(' parameters ')' '->' types ':' statement*;
parameters: parameter (',' parameter)*;
parameter: IDENTIFIER ':' types ('=' expression);

expression
    : primary
    | composite;

composite : primary? (
        '/'  | '*'  |
        '>>' | '<<' | '+' | '-' | '//' | '**' |
        '==' | '!=' | '<' | '>' | '<=' | '>=' |
        'AND'| 'OR'
        ) primary?;     // both primaries are optional, but the parser enforces
                        // at least 1

literal
    : 'True'
    | 'False'
    | CHARACTER
    | '-'? INTEGER
    | '-'? FLOAT
    | string;

primary
    : IDENTIFIER type_params? '(' arguments ')'  // function call
    | IDENTIFIER
    | literal
    | '(' expression ')'
    | '[' expression (',' expression)* ']'
    | '{' key_value (',' key_value)* '}'; // list

arguments: argument (',' arguments)*;
argument: expression;

key_value: expression ':' expression;

// The expression will later HAVE to have an iterator type
for: 'for' IDENTIFIER 'in' expression ':' statement+ loop_else?;

if_statement: 'if' expression ':' statement+ else_statement?;
else_statement
    : 'else' ':' statement+
    | 'elif' expression ':' statement+ else_statement?;

match: 'match' expression ':' case+;
case: 'case' pattern ('|' pattern)* ':';
pattern
    : IDENTIFIER
    | literal
    // Sequence:
    // NOTE: an element of a sequence may be preceded by a star under specific
    // requirements, which the parser enforces but are not written out here.
    //  1. Can only be followed by a literal or an identifier (not a sequence)
    //  2. Can only occur once per sequence
    | ('(' | '[') '*'? pattern (',' '*'? pattern)* (')' | ']');

while: 'while' expression ':' statement+ loop_else?;
loop_else: 'else' ':' statement+;

/* JUST GAWDDADN STRINGS */
string
    : STRING
    | LONG_STRING
    | string_prefix string_prefix? (STRING | LONG_STRING);
    // you can combine string prefixes, tho combinations like ff should not
    // (and don't) work

string_prefix: ('f'|'F'|'b'|'B'|'r'|'R');
