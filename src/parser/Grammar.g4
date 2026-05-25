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
    | return;

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
    : IDENTIFIER
    | CHARACTER
    | INTEGER
    | FLOAT
    | '(' expression ')';
