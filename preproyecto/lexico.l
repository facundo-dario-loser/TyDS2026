%{
    // ejercicio 3)
    // definitions
    #include <stdio.h>
    #include <stdlib.h>
    
    enum tokens {
        INT = 256,
        BOOL,
        VOID,
        RETURN,
        ID,
        CONSTANTE_NUMERICA,
        CONSTANTE_BOOLEANA,
        PLUS,
        MULT,
        ASSIGMENT,
        LPAREN,
        RPAREN,
        LCURLY,
        RCURLY,
        SEMICOLON,
        MAIN
    };
%}

%option noyywrap

%%
int                  { printf("INT\n");                                  return INT; }
bool                 { printf("BOOL\n");                                 return BOOL; }
void                 { printf("VOID\n");                                 return VOID; }
return               { printf("RETURN\n");                               return RETURN; }
main                 { printf("MAIN\n");                                 return MAIN; }
true|false           { printf("CONSTANTE_BOOLEANA(%s)\n", yytext);       return CONSTANTE_BOOLEANA; }
[a-zA-Z][a-zA-Z0-9]* { printf("ID(%s)\n", yytext);                       return ID; }
[0-9]+               { printf("CONSTANTE_NUMERICA(%d)\n", atoi(yytext)); return CONSTANTE_NUMERICA; }
"+"                  { printf("PLUS\n");                                 return PLUS; }
"*"                  { printf("MULT\n");                                 return MULT; }
"="                  { printf("ASSIGMENT\n");                            return ASSIGMENT; }
"("                  { printf("LPAREN\n");                               return LPAREN; }
")"                  { printf("RPAREN\n");                               return RPAREN; }
"{"                  { printf("LCURLY\n");                               return LCURLY; }
"}"                  { printf("RCURLY\n");                               return RCURLY; }
";"                  { printf("SEMICOLON\n");                            return SEMICOLON; }
[ \n\t]+             { /* ignorar espacios, new line y tab */ }
%%

void main(int argc, char** argv) {
    ++argv;
    --argc;

    if (argc > 0) {
        yyin = fopen(argv[0], "r");
    } else {
        yyin = stdin;
    }

    while (yylex() != 0) {}
}

