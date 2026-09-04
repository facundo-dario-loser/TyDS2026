#include "ast.h"

ASTNode * newLeaf(ASTLeafConfig *config) {
    ASTNode *n = (ASTNode*)malloc(sizeof(ASTNode));
    CHECK_IS_NOT_NULL(n);
    n->tipo         = config->tipo;
    n->valor        = config->valor;
    if (config->nombre) n->nombre = strdup(config->nombre);
    n->left         = NULL;
    n->right        = NULL;
    n->semanticType = config->semanticType;
    n->line         = config->line;
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
    if (root) {
        freeAST(root->left);
        freeAST(root->right);

        if (root->nombre) free(root->nombre);

        if (root->simbolo) {
            root->simbolo->refCount--;
            if (root->simbolo->refCount == 0) freeSymbol(root->simbolo);
        }

        free(root);
    }
}

void printASTAux(ASTNode *root) {
    if (!root) return;

    if (IS_LEAF(root)) {
        switch (root->tipo) {
            case NODE_CTE_INT:   { printf("CTE(%d)", root->valor); break; }
            case NODE_CTE_BOOL:  { printf("CTE(%s)", root->valor ? "true" : "false"); break; }
            case NODE_ID:        { printf("ID(%s)", root->nombre); break; }
            case NODE_TYPE:      { char *type;
                                switch (root->semanticType) {
                                    case SEMANTIC_TYPE_INT:  type = "int";  break;
                                    case SEMANTIC_TYPE_BOOL: type = "bool"; break;
                                    case SEMANTIC_TYPE_VOID: type = "void"; break;
                                }
                                printf("TYPE(%s)", type); break; 
                              }
        } 
    } else {
        printf("(");
        if (root->left) printASTAux(root->left); // izq
        printf(" ");

        switch (root->tipo) { // root
            case NODE_PROG:      { printf("PROG"); break; }
            case NODE_CUERPO:    { printf("CUERPO"); break; }
            case NODE_DECL:      { printf("DECL"); break; }
            case NODE_EXP_SUMA:  { printf("+"); break; }
            case NODE_EXP_MULT:  { printf("*"); break; }
            case NODE_EXP_AND:   { printf("&&"); break; }
            case NODE_EXP_OR:    { printf("||"); break; }
            case NODE_ASSIGN:    { printf("="); break; }
            case NODE_RETURN:    { printf("RETURN"); break; }
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
