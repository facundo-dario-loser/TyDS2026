#include "ast.h"

ASTNode * newLeaf(ASTLeafConfig *config) {
    ASTNode *n = (ASTNode*)malloc(sizeof(ASTNode));
    CHECK_IS_NOT_NULL(n);
    n->tipo   = config->tipo;
    n->valor  = config->valor;
    if (config->nombre) n->nombre = strdup(config->nombre);
    n->left   = NULL;
    n->right  = NULL;
    n->semanticType = config->semanticType;
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

// setea en null el puntero al simbolo de todos los nodos del arbol que apunten al mismo simbolo que symbolPointer, menos root
void setAllSymbolPointerNULL(ASTNode *current, Symbol *symbolPointer) {
    if (!current || !symbolPointer) return;
    
    if (current->simbolo == symbolPointer) current->simbolo = NULL;
    
    setAllSymbolPointerNULL(current->left, symbolPointer);
    setAllSymbolPointerNULL(current->right, symbolPointer);
}

void freeASTAux(ASTNode *node, ASTNode *root) {
    if (node) { // si no es NULL libera la memoria

        freeAST(node->left);
        freeAST(node->right);

        if (root->nombre) free(root->nombre); // es seguro solo si root->nombre apunta a una cadena creada con malloc
        
        if (root->simbolo) {
            // freeASTAux libera bottom->up, pero debo setear los punteros a simbolos up->bottom
            setAllSymbolPointerNULL(root, node->simbolo);
            freeSymbol(root->simbolo);
            root->simbolo = NULL;
        }

        free(node);
    }
}

void freeAST(ASTNode * root) {
    freeASTAux(root, root);
}

void printASTAux(ASTNode *root) {
    if (!root) return;

    if (IS_LEAF(root)) {
        switch (root->tipo) {
            case N_CTE_INT:   { printf("CTE(%d)", root->valor); break; }
            case N_CTE_BOOL:  { printf("CTE(%s)", root->valor ? "true" : "false"); break; }
            case N_ID:        { printf("ID(%s)", root->nombre); break; }
            case N_TYPE:      { char *type;
                                switch (root->semanticType) {
                                    case S_INT:  type = "int";  break;
                                    case S_BOOL: type = "bool"; break;
                                    case S_VOID: type = "void"; break;
                                }
                                printf("TYPE(%s)", type); break; 
                              }
        } 
    } else {
        printf("(");
        if (root->left) printASTAux(root->left); // izq
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
        if (root->right) printASTAux(root->right); // der
        printf(")");
    }
}

void printAST(ASTNode *root) {
    printf("\n[LOG:AST]:\n");
    printASTAux(root);
    printf("\n\n");
}
