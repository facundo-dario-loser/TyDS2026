#include <stdio.h>
#include "TADs/ast.h"
#include "TADs/ts.h"
#include "analizador_semantico/analisis_semantico.h"
#include "interprete/interprete.h"
#include "pseudo_asm/ir_asm.h"

extern ASTNode *root;
extern FILE    *yyin;
extern int     yyparse();

typedef enum ArgumentFlag {
    INTERPRETE,
    GENERADOR_PSEUDO_ASSEMBLY
} ArgumentFlag;

ArgumentFlag flag;

ArgumentFlag getArgumentFlag(int argc, char **argv);

int main(int argc, char **argv) {
    ++argv;
    --argc;

    flag = getArgumentFlag(argc, argv);

    if (argc >= 2) {
        yyin = fopen(argv[1], "r");

        if (!yyin) {
            printf("[ERROR]: no se pudo abrir el archivo: '%s'\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("[ERROR]: no se proporciono ningun archivo\n");
        exit(EXIT_FAILURE);
    }

    // analisis sintactico
    int res = yyparse();
    if (res == 0) {
        printf("[LOG]: analisis sintactico completado\n");
    } else {
        printf("[ERROR:ANALISIS:SINTACTICO]: hubo un error (yyparse() = 0)\n");
    }

    #ifdef DEBUG_AST
        printAST(root);
    #endif

    analisisSemantico(root);
    printf("[LOG]: analisis semantico completado\n");

    if (flag == INTERPRETE) {
        interprete(root);
        printf("[LOG]: el interprete finalizo su ejecucion\n");
    } else {
        Instruction *head = generarPseudoAsmList(root);
        printf("[LOG]: generacion de pseudo assembly completado\n"); 
        //writeInstructions(head);
        printInstructions(head);
        freeInstructionList(head); // NOTA: cuando genere assembly no deberia liberar la lista aca
    }

    // liberamos la memoria de todos los nodos del arbol (y por ende todos los simbolos creados)
    freeAST(root);

    return 0;
}

ArgumentFlag getArgumentFlag(int argc, char **argv) {
    if (!(argc >= 1)) {
        printf("[ERROR]: no se indico ningun flag. Uso:\n");
        printf("\t\t-i para ejecutar el interprete\n");
        printf("\t\t-s para ejecutar el generador de pseudo assembly\n");
        exit(EXIT_FAILURE);
    } else {
        if (strcmp(argv[0], "-i") == 0) {
            return INTERPRETE;
        } else if (strcmp(argv[0], "-p") == 0) {
            return GENERADOR_PSEUDO_ASSEMBLY;
        } else {
            printf("[ERROR]: no se indico ningun flag. Uso:\n");
            printf("\t\t-i para ejecutar el interprete\n");
            printf("\t\t-s para ejecutar el generador de pseudo assembly\n");
            exit(EXIT_FAILURE);
        }
    }
}
