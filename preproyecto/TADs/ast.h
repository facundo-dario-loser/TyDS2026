#ifndef AST_H
#define AST_H

// Arbol sintactico abstracto

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ts.h"

#define CHECK_IS_NOT_NULL(node) if (!n) { printf("[ERROR:AST]: no se pudo alocar memoria para un nodo\n"); exit(EXIT_FAILURE); }

typedef enum ASTNodeType {
    NODE_PROG,     // programa (que por ahora es solo la funcion main)
    NODE_TYPE,     // int, bool, void
    NODE_CUERPO,   // cuerpo de una funcion (solo main por ahora)
    NODE_DECL,     // declaracion
    NODE_EXP_SUMA, // expresion +
    NODE_EXP_MULT, // expresion *
    NODE_EXP_AND,  // expresion &&
    NODE_EXP_OR,   // expresion ||
    NODE_CTE_INT,  // constante numerica
    NODE_CTE_BOOL, // constante booleana
    NODE_ID,       // id de variables
    NODE_ASSIGN,   // =
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
    bool           tieneReturn;  // permite saber si en una rama existe un return (es util para analizar el caso en 
                                 // que main retorna algo y entonces chequear que en el cuerpo main efectivamente se retorne una expresion de ese mismo tipo)
    int            line;         // linea en el prog del token
    int            valorExp;     // para guardar el resultado de expresion (+, *, &&, ||). se usa en el interprete
} ASTNode;

// estructura para rellenar los campos al crear un nodo hoja y pasarsela a newLeaf
// permite usar una unica funcion para crear una hoja (ya que puedo usar solo los campos que me interesan)
// y evito tener muchas funciones para crear cada tipo de nodo o tener 1 funcion con muchos parametros
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