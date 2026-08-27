%{
#include <stdio.h>
#include <stdlib.h>

#define AST_IMPLEMENTATION
#include "../TADs/ast.h"

int         yylex(void);
extern void yyerror(const char *s);
extern FILE *yyin;

ASTNode *root = NULL;
%}

// para que meta ast.h en parser.tab.h
%code requires {
    #include "../TADs/ast.h"
}

// valores/datos que peuden tener los tokens
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
p: type MAIN '(' ')' '{' c '}' { $$ = newTree(N_PROG, $1, $6);
                                 root = $$;
                               }
;

type: INT  { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_TYPE, .nombre = "int"});  }
    | BOOL { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_TYPE, .nombre = "bool"}); }
    | VOID { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_TYPE, .nombre = "void"}); }
    ;

c: d c          { $$ = newTree(N_CUERPO, $1, $2); }
 | s c          { $$ = newTree(N_CUERPO, $1, $2); }
 | /* lambda */ { $$ = NULL; }
 ;

e: e '+' e            { $$ = newTree(N_EXP_SUMA, $1, $3); }
 | e '*' e            { $$ = newTree(N_EXP_MULT, $1, $3); }
 | e AND e            { $$ = newTree(N_EXP_AND, $1, $3); }
 | e OR e             { $$ = newTree(N_EXP_OR, $1, $3); }
 | '(' e ')'          { $$ = $2; }
 | CONSTANTE_NUMERICA { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_CTE_INT, .valor = $1}); }
 | CONSTANTE_BOOLEANA { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_CTE_BOOL, .valor = $1}); }
 | ID                 { $$ = newLeaf(&(ASTLeafConfig){.tipo = N_ID, .nombre = $1}); }
 ;

s: ID '=' e ';' { ASTNode *leaf = newLeaf(&(ASTLeafConfig){.tipo = N_ID, .nombre = $1});
                  $$ = newTree(N_ASSIGN, leaf, $3); 
                }
 | RETURN e ';' { $$ = newTree(N_RETURN, $2, NULL); }
 | RETURN ';'   { $$ = newTree(N_RETURN, NULL, NULL); }
 ;

d: type ID ';' { ASTNode *leaf = newLeaf(&(ASTLeafConfig){.tipo = N_ID, .nombre = $2}); 
                  $$ = newTree(N_DECL, $1, leaf); 
                }
;

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

    if (res == 0) printf("[LOG]: analisis sintactico completado\n");
    printf("****AST****:\n");
    printAST(root);

    return res;
}
