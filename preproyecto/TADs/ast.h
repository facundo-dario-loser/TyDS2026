#ifndef AST_H
#define AST_H

// Arbol abstracto sintactico

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ts.h"

#define CHECK_IS_NOT_NULL(node) if (!n) { printf("[ERROR:AST]: no se pudo alocar memoria para un nodo\n"); exit(EXIT_FAILURE); }
#define IS_LEAF(node)           !node->left && !node->right

typedef enum ASTNodeType {
    NODE_PROG,     // programa
    NODE_TYPE,     // (int, bool, void)
    NODE_CUERPO,   // cuerpo de una funcion (solo main por ahora)
    NODE_DECL,     // declaracion
    NODE_EXP_SUMA, // expresion +
    NODE_EXP_MULT, // expresion *
    NODE_EXP_AND,  // expresion &&
    NODE_EXP_OR,   // expresion ||
    NODE_CTE_INT,  // constante numerica
    NODE_CTE_BOOL, // constante booleana
    NODE_ID,
    NODE_ASSIGN,
    NODE_RETURN,
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
    int            valorExp;     // para guardar el resultado de expresion (+, *, &&, ||). se usa en el interprete
} ASTNode;

// estructura para rellenar los campos al crear un nodo hoja y pasarsela a newLeaf
// permite usar una unica funcion para crear una hoja (ya que puedo usar solo los campos que me interesan)
typedef struct ASTLeafConfig {
    ASTNodeType  tipo;
    int          valor;
    char         *nombre;
    SemanticType semanticType;
    int          line;
} ASTLeafConfig;

ASTNode * newLeaf(ASTLeafConfig *config);

ASTNode * newTree(ASTNodeType tipo, ASTNode *left, ASTNode *right);

void freeAST(ASTNode * root);

// printea el AST estilo haskell
void printAST(ASTNode *root);

#endif // AST_H