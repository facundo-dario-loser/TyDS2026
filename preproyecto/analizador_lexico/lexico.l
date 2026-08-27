%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "parser.tab.h"
%}

%option noyywrap

%%
int                  { printf("INT\n");                                                                  return INT; }
bool                 { printf("BOOL\n");                                                                 return BOOL; }
void                 { printf("VOID\n");                                                                 return VOID; }
return               { printf("RETURN\n");                                                               return RETURN; }
main                 { printf("MAIN\n");                                                                 return MAIN; }
true                 { printf("CONSTANTE_BOOLEANA(%s)\n", yytext);       yylval.valor = 1;               return CONSTANTE_BOOLEANA; }
false                { printf("CONSTANTE_BOOLEANA(%s)\n", yytext);       yylval.valor = 0;               return CONSTANTE_BOOLEANA; }
[a-zA-Z][a-zA-Z0-9]* { printf("ID(%s)\n", yytext);                       yylval.nombre = strdup(yytext); return ID; }
[0-9]+               { printf("CONSTANTE_NUMERICA(%d)\n", atoi(yytext)); yylval.valor = atoi(yytext);    return CONSTANTE_NUMERICA; }
"+"                  { printf("PLUS\n");                                                                 return '+'; }
"*"                  { printf("MULT\n");                                                                 return '*'; }
"&&"                 { printf("AND\n");                                                                  return AND; }
"||"                 { printf("OR\n");                                                                   return OR; }
"="                  { printf("ASSIGMENT\n");                                                            return '='; }
"("                  { printf("LPAREN\n");                                                               return '('; }
")"                  { printf("RPAREN\n");                                                               return ')'; }
"{"                  { printf("LCURLY\n");                                                               return '{'; }
"}"                  { printf("RCURLY\n");                                                               return '}'; }
";"                  { printf("SEMICOLON\n");                                                            return ';'; }
[ \n\t]+             { /* ignorar espacios, new line y tab */ }
"//".*               { printf("C_LINEA(%s)\n", yytext); /* se ignora */ }
.                    { printf("[ERROR]: token invalido (%s)\n", yytext); exit(-1); }
%%
