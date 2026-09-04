%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../analizador_sintactico/parser.tab.h"

    #ifdef DEBUG_LEXICO
        #define debug_lexico(...) printf(__VA_ARGS__);
    #else
        #define debug_lexico(...)
    #endif
%}

%option noyywrap
%option yylineno

%%
int                  { debug_lexico("INT\n");                                                                  return INT; }
bool                 { debug_lexico("BOOL\n");                                                                 return BOOL; }
void                 { debug_lexico("VOID\n");                                                                 return VOID; }
return               { debug_lexico("RETURN\n");                                                               return RETURN; }
main                 { debug_lexico("MAIN\n");                                                                 return MAIN; }
true                 { debug_lexico("CONSTANTE_BOOLEANA(%s)\n", yytext);       yylval.valor = 1;               return CONSTANTE_BOOLEANA; }
false                { debug_lexico("CONSTANTE_BOOLEANA(%s)\n", yytext);       yylval.valor = 0;               return CONSTANTE_BOOLEANA; }
[a-zA-Z][a-zA-Z0-9]* { debug_lexico("ID(%s)\n", yytext);                       yylval.nombre = strdup(yytext); return ID; }
[0-9]+               { debug_lexico("CONSTANTE_NUMERICA(%d)\n", atoi(yytext)); yylval.valor = atoi(yytext);    return CONSTANTE_NUMERICA; }
"+"                  { debug_lexico("PLUS\n");                                                                 return '+'; }
"*"                  { debug_lexico("MULT\n");                                                                 return '*'; }
"&&"                 { debug_lexico("AND\n");                                                                  return AND; }
"||"                 { debug_lexico("OR\n");                                                                   return OR; }
"="                  { debug_lexico("ASSIGMENT\n");                                                            return '='; }
"("                  { debug_lexico("LPAREN\n");                                                               return '('; }
")"                  { debug_lexico("RPAREN\n");                                                               return ')'; }
"{"                  { debug_lexico("LCURLY\n");                                                               return '{'; }
"}"                  { debug_lexico("RCURLY\n");                                                               return '}'; }
";"                  { debug_lexico("SEMICOLON\n");                                                            return ';'; }
[ \n\t]+             { /* ignorar espacios, new line y tab */ }
"//".*               { debug_lexico("C_LINEA(%s)\n", yytext); /* se ignora */ }
.                    { debug_lexico("[ERROR]: token invalido (%s)\n", yytext); exit(EXIT_FAILURE); }
%%
