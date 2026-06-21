/*
 * THIS GRAMMAR IS A REPRESENTATION OF WHAT THE PARSER CLASS DOES, IT IS NOT
 * ACTAULLY USED TO GENERATE THE PARSER CLASS. THAT IS ALSO THE REASON SOME
 * ANTLR4 SYNTAX ERRORS CAN BE FOUND (CUZ I CAN'T BE BOTHERED)
**/

start: statement* EOF;


statement
    : assignment
    | COMMENT
    | for
    | function
    | if_statement
    | while
    | return;   // Parser does have a check for whether this is inside a function

assignment: IDENTIFIER (':' TYPE)? (('=' expression)? | compound_assignment expression);
compound_assignment: '+=' | '-=' | '/=' | '*=' | '**=' | '//=' | '%='

function: 'def' IDENTIFIER '(' parameters ')' '->' TYPE ':' statement*;
parameters: parameter (',' parameter)*;
parameter: IDENTIFIER ':' TYPE ('=' expression);

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

primary
    : IDENTIFIER '(' arguments ')'  // function call
    | IDENTIFIER
    | 'True'
    | 'False'
    | CHARACTER
    | INTEGER
    | FLOAT
    | string
    | '(' expression ')';

arguments: argument (',' arguments)*;
argument: expression;

// The expression will later HAVE to have an iterator type
for: 'for' IDENTIFIER 'in' expression ':' statement+ loop_else?;

if_statement: 'if' expression ':' statement+ else_statement?;
else_statement
    : 'else' ':' statement+
    | 'elif' expression ':' statement+ else_statement?;

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
