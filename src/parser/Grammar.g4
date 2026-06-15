/*
 * THIS GRAMMAR IS A REPRESENTATION OF WHAT THE PARSER CLASS DOES, IT IS NOT
 * ACTAULLY USED TO GENERATE THE PARSER CLASS. THAT IS ALSO THE REASON SOME
 * ANTLR4 SYNTAX ERRORS CAN BE FOUND (CUZ I CAN'T BE BOTHERED)
**/

start: statement* EOF;


/* STATEMENTS */
statement
    : assignment
    | COMMENT
    | function
    | if_statement
    | return;   // Parser does have a check for whether this is inside a function

assignment: IDENTIFIER (':' TYPE)? ('=' expression)?;

function: 'def' IDENTIFIER '(' parameters ')' '->' TYPE ':' statement*;
parameters: parameter (',' parameter)*;
parameter: IDENTIFIER ':' TYPE ('=' expression);

expression
    : primary
    | composite;

composite : primary? (
        '/'  | '*'  |
        '>>' | '<<' | '+' | '-' | '//' | '**'
        '==' | '!=' | '<' | '>' | '<=' | '>='
        ) primary?;     // both primaries are optional, but the parser enforces
                        // at least 1

primary
    : IDENTIFIER '(' arguments ')'
    | IDENTIFIER
    | CHARACTER
    | INTEGER
    | FLOAT
    | '(' expression ')';

arguments: argument (',' arguments)*;
argument: expression;

if_statement: 'if' expression ':' statement+ else_statement?
else_statement
    : 'else' ':' statement+
    | 'elif' expression ':' statement+ else_statement?
