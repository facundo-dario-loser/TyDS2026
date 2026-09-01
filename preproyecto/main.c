#include <stdio.h>
#include "TADs/ast.h"
#include "TADs/ts.h"
#include "analizador_semantico/analisis_semantico.h"
#include "interprete/interprete.h"

extern ASTNode *root;
extern FILE    *yyin;
extern int     yyparse();

int main(int argc, char **argv) {
    ++argv;
    --argc;

    if (argc > 0) {
        yyin = fopen(argv[0], "r");
    }

    int res = yyparse();

    if (res == 0) printf("[LOG]: analisis sintactico completado\n");
    
    #ifdef DEBUG_AST
        printf("\n****AST****:\n");
        printAST(root);
        printf("\n");
    #endif

    analisisSemantico(root);
    printf("[LOG]: analisis semantico completado\n");

    interprete(root);

    return res;
}