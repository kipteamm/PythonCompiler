/*
 * THIS GRAMMAR IS A REPRESENTATION OF WHAT THE PARSER CLASS DOES, IT IS NOT
 * ACTAULLY USED TO GENERATE THE PARSER CLASS. THAT IS ALSO THE REASON SOME
**/

start: statement* EOF;


/* STATEMENTS */
statement
    : assignment
    | COMMENT
    | function;

assignment: IDENTIFIER (':' TYPE)? ('=' expression)?;

function: 'def' IDENTIFIER '(' parameters ')' '->' TYPE ':' statement*;
parameters: parameter (',' parameter)*;
parameter: IDENTIFIER ':' TYPE ('=' expression);

expression
    : IDENTIFIER
    | CHARACTER
    | FLOAT
    | INTEGER
    | STRING;
