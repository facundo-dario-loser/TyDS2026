#ifndef AST_H
#define AST_H

// Arbol abstracto sintactico

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ts.h"

#define CHECK_IS_NOT_NULL(node) if (!n) { printf("[ERROR]: no se pudo alocar memoria para un nodo\n"); exit(-1); }
#define IS_LEAF(node) !node->left && !node->right

typedef enum ASTNodeType {
    N_PROG,     // programa
    N_TYPE,     // (int, bool, void)
    N_CUERPO,   // cuerpo de main
    N_DECL,     // declaracion
    N_EXP_SUMA, // expresion +
    N_EXP_MULT, // expresion *
    N_EXP_AND,  // expresion &&
    N_EXP_OR,   // expresion ||
    N_CTE_INT,  // constante numerica
    N_CTE_BOOL, // constante booleana
    N_ID,
    N_ASSIGN,
    N_RETURN,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType    tipo;
    int            valor;        // para numeros y booleanos
    char           *nombre;
    struct ASTNode *left;
    struct ASTNode *right;
    Symbol         *simbolo;
    SemanticType   semanticType; // para saber si la expresion/id es int-bool-void
    bool           tieneReturn;  // permite saber si en una rama existe un return (es util para analizar semanticamente los valroes de retorno de funciones)
    int            line;         // linea en el prog del token
} ASTNode;

// estructura para rellenar los campos al crear un nodo hoja y pasarle la estructura a newLeaf
typedef struct ASTLeafConfig {
    ASTNodeType  tipo;
    int          valor;
    char         *nombre;
    SemanticType semanticType;
    int          line; //?? creo que no hace falta
} ASTLeafConfig;

// crear hoja
ASTNode * newLeaf(ASTLeafConfig *config);

// crear un arbol binario
ASTNode * newTree(ASTNodeType tipo, ASTNode *left, ASTNode *right);

// destruye el AST y libera toda la memoria
void freeAST(ASTNode * root);

// printea el AST estilo haskell
void printAST(ASTNode *root);

#endif // AST_H