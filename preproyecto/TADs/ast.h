#ifndef AST_H
#define AST_H

// Arbol abstracto sintactico

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    int            valor; // para guardar constantes numericas y booleanas
    char           *nombre;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

// estructura para rellenar los campos al crear un nodo hoja y pasarle la estructura a newLeaf
typedef struct ASTLeafConfig {
    ASTNodeType tipo;
    int         valor;
    char        *nombre;
} ASTLeafConfig;

// crear hoja
ASTNode * newLeaf(ASTLeafConfig *config);

// crear un arbol binario
ASTNode * newTree(ASTNodeType tipo, ASTNode *left, ASTNode *right);

// destruye el AST y libera toda la memoria
void freeAST(ASTNode * root);

// printea el AST estilo haskell
void printAST(ASTNode *root);

#ifdef AST_IMPLEMENTATION

ASTNode * newLeaf(ASTLeafConfig *config) {
    ASTNode *n = (ASTNode*)malloc(sizeof(ASTNode));
    CHECK_IS_NOT_NULL(n);
    n->tipo   = config->tipo;
    n->valor  = config->valor;
    if (config->nombre) n->nombre = strdup(config->nombre);
    n->left   = NULL;
    n->right  = NULL;
    return n;
}

ASTNode * newTree(ASTNodeType tipo, ASTNode *left, ASTNode *right) {
    ASTNode *n = (ASTNode*)malloc(sizeof(ASTNode));
    CHECK_IS_NOT_NULL(n);
    n->tipo  = tipo;
    n->left  = left;
    n->right = right;
    return n;
}

void freeAST(ASTNode * root) {
    if (root) { // si no es NULL libera la memoria
        freeAST(root->left);
        freeAST(root->right);
        free(root);
    }
}

void printAST(ASTNode *root) {
    if (!root) return;

    if (IS_LEAF(root)) {
        switch (root->tipo) {
            case N_CTE_INT:   { printf("CTE(%d)", root->valor); break; }
            case N_CTE_BOOL:  { printf("CTE(%s)", root->valor ? "true" : "false"); break; }
            case N_ID:        { printf("ID(%s)", root->nombre); break; }
            case N_TYPE:      { printf("TYPE(%s)", root->nombre); break; }
        } 
    } else {
        printf("(");
        if (root->left) printAST(root->left); // izq
        printf(" ");

        switch (root->tipo) { // root
            case N_PROG:      { printf("PROG"); break; }
            case N_CUERPO:    { printf("CUERPO"); break; }
            case N_DECL:      { printf("DECL"); break; }
            case N_EXP_SUMA:  { printf("+"); break; }
            case N_EXP_MULT:  { printf("*"); break; }
            case N_EXP_AND:   { printf("&&"); break; }
            case N_EXP_OR:    { printf("||"); break; }
            case N_ASSIGN:    { printf("="); break; }
            case N_RETURN:    { printf("RETURN"); break; }
        }

        printf(" ");
        if (root->right) printAST(root->right); // der
        printf(")");
    }
}

#endif // AST_IMPLEMENTATION

#endif // AST_H