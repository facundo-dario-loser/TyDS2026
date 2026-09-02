%{
#include <stdio.h>
#include <stdlib.h>
#include "../TADs/ast.h"

int  yylex(void);
void yyerror(const char *s);

extern FILE *yyin;
extern int  yylineno;
ASTNode     *root = NULL;
%}

// para hacer mejores logs cuando hay un error de sintaxis
%define parse.error detailed 

// para que meta las bibliotecas en parser.tab.h
%code requires {
    #include "../TADs/ast.h"
}

// valores/datos que pueden tener los tokens
%union {
    int     valor;
    char    *nombre;
    ASTNode *node;
}

%token <nombre> ID
%token <valor> CONSTANTE_NUMERICA CONSTANTE_BOOLEANA
%token INT BOOL VOID RETURN AND OR MAIN

%type <node> p c s d e type

// precendencia (de menor a mayor)
%left OR
%left AND
%left '+'
%left '*'

%%

// convencion: tokens (terminales) van en mayus y los no terminales en minus
p: type MAIN '(' ')' '{' c '}' { 
                                 $$ = newTree(N_PROG, $1, $6);
                                 $$->line = $1->line; // la linea donde arranca main (bison hace LALR por ende 'p' se termina de procesar al final y termina arrojando la ultima linea si no hago esto)
                                 root = $$;
                               }
;

type: INT  { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_TYPE, .semanticType = S_INT});  $$->line = yylineno; }
    | BOOL { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_TYPE, .semanticType = S_BOOL}); $$->line = yylineno; }
    | VOID { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_TYPE, .semanticType = S_VOID}); $$->line = yylineno; }
    ;

c: d c          { $$ = newTree(N_CUERPO, $1, $2); }
 | s c          { $$ = newTree(N_CUERPO, $1, $2); }
 | /* lambda */ { $$ = NULL; }
 ;

e: e '+' e            { $$ = newTree(N_EXP_SUMA, $1, $3); $$->line = yylineno; }
 | e '*' e            { $$ = newTree(N_EXP_MULT, $1, $3); $$->line = yylineno; }
 | e AND e            { $$ = newTree(N_EXP_AND, $1, $3); $$->line = yylineno; }
 | e OR e             { $$ = newTree(N_EXP_OR, $1, $3); $$->line = yylineno; }
 | '(' e ')'          { $$ = $2; }
 | CONSTANTE_NUMERICA { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_CTE_INT, .valor = $1, .semanticType = S_INT}); }
 | CONSTANTE_BOOLEANA { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_CTE_BOOL, .valor = $1, .semanticType = S_BOOL}); }
 | ID                 { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_ID, .nombre = $1}); $$->line = yylineno; }
 ;

s: ID '=' e ';' { 
                  ASTNode *leaf = newLeaf(&(ASTLeafConfig){.tipo = N_ID, .nombre = $1});
                  leaf->line = yylineno;
                  $$ = newTree(N_ASSIGN, leaf, $3); 
                  $$->line = yylineno;
                }
 | RETURN e ';' { $$ = newTree(N_RETURN, $2, NULL); $$->line = yylineno; }
 | RETURN ';'   { $$ = newTree(N_RETURN, NULL, NULL); $$->line = yylineno; }
 ;

d: type ID ';' { 
                  ASTNode *leaf = newLeaf(&(ASTLeafConfig){.tipo = N_ID, .nombre = $2}); 
                  $$ = newTree(N_DECL, $1, leaf); 
                  $$->line = yylineno;
                }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[SINTAX ERROR]: %s (linea: %d)\n", s, yylineno);
}

