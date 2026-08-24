%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
extern void yyerror(const char *s);
extern FILE *yyin;
// convencion: tokens (terminales) van en mayus y los no terminales en minus
%}

%token INT BOOL VOID RETURN ID CONSTANTE_NUMERICA CONSTANTE_BOOLEANA AND OR MAIN

%left OR
%left AND
%left '+'
%left '*'

%%

p: type MAIN '(' ')' '{' a '}';

type: INT
    | BOOL
    | VOID 
    ;

a: d a 
 | s a
 | /* lambda */
 ;

e: e '+' e
 | e '*' e
 | e AND e
 | e OR e
 | '(' e ')'
 | CONSTANTE_NUMERICA
 | CONSTANTE_BOOLEANA
 | ID
 ;

s: ID '=' e ';'
 | RETURN e ';'
 | RETURN ';'
 ;

d: type ID ';';

%%

void yyerror(const char *s) {
    fprintf(stderr, "[SINTAX ERROR]: %s\n", s);
}

int main(int argc, char** argv) {
    ++argv;
    --argc;

    if (argc > 0) {
        yyin = fopen(argv[0], "r");
    } else {
        yyin = stdin;
    }

    int res = yyparse();

    if (res == 0) printf("[LOG]: analisis lexico completado\n");
    return res;
}
