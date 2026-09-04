#include <stdio.h>
#include "TADs/ast.h"
#include "TADs/ts.h"
#include "analizador_semantico/analisis_semantico.h"
#include "interprete/interprete.h"
#include "pseudo_asm/ir_asm.h"

extern ASTNode *root;
extern FILE    *yyin;
extern int     yyparse();

int main(int argc, char **argv) {
    ++argv;
    --argc;

    if (argc > 0) {
        yyin = fopen(argv[0], "r");
        if (!yyin) {
            printf("[ERROR]: no se pudo abrir el archivo: '%s'\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("[ERROR]: no se proporciono ningun archivo\n");
        exit(EXIT_FAILURE);
    }

    int res = yyparse();
    if (res == 0) printf("[LOG]: analisis sintactico completado\n");

    #ifdef DEBUG_AST
        printAST(root);
    #endif

    analisisSemantico(root);
    printf("[LOG]: analisis semantico completado\n");

    //interprete(root);
    generarPseudoAsm(root);

    return res;
}