#include "interprete.h"

void interpretarNodeExpSuma(ASTNode *node);
void interpretarNodeExpMult(ASTNode *node);
void interpretarNodeExpAnd(ASTNode *node);
void interpretarNodeExpOr(ASTNode *node);
void interpretarNodeCteInt(ASTNode *node);
void interpretarNodeCteBool(ASTNode *node);
void interpretarNodeId(ASTNode *node);
void interpretarNodeAssign(ASTNode *node);
void interpretarNodeReturn(ASTNode *node);

void interprete(ASTNode *root) {
    if (!root) return; // para los hijos NULL

    switch (root->tipo) {
        case NODE_PROG:     interprete(root->right);      break;
        case NODE_TYPE:                                   break;
        case NODE_CUERPO:   interprete(root->left);
                            interprete(root->right);      break;
        case NODE_DECL:                                   break;
        case NODE_EXP_SUMA: interpretarNodeExpSuma(root); break;
        case NODE_EXP_MULT: interpretarNodeExpMult(root); break;
        case NODE_EXP_AND:  interpretarNodeExpAnd(root);  break;
        case NODE_EXP_OR:   interpretarNodeExpOr(root);   break;
        case NODE_CTE_INT:  interpretarNodeCteInt(root);  break;
        case NODE_CTE_BOOL: interpretarNodeCteBool(root); break;
        case NODE_ID:       interpretarNodeId(root);      break;
        case NODE_ASSIGN:   interpretarNodeAssign(root);  break;
        case NODE_RETURN:   interpretarNodeReturn(root);  break;
    }
}

void interpretarNodeExpSuma(ASTNode *node) {
    debug_interprete("NEXPSUMA")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp + node->right->valorExp;
}

void interpretarNodeExpMult(ASTNode *node) {
    debug_interprete("NEXPMULT")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp * node->right->valorExp;
}

void interpretarNodeExpAnd(ASTNode *node) {
    debug_interprete("NEXPAND")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp && node->right->valorExp;
}

void interpretarNodeExpOr(ASTNode *node) {
    debug_interprete("NEXPOR")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp || node->right->valorExp;
}

void interpretarNodeCteInt(ASTNode *node) {
    debug_interprete("NCTEINT")
    node->valorExp = node->valor;
}

void interpretarNodeCteBool(ASTNode *node) {
    debug_interprete("NCTEBOOL")
    node->valorExp = node->valor;
}

void interpretarNodeId(ASTNode *node) {
    debug_interprete("NID")
    node->valorExp = node->simbolo->valor;
}

void interpretarNodeAssign(ASTNode *node) {
    debug_interprete("NASSIGN")
    interprete(node->right);
    node->left->simbolo->valor = node->right->valorExp;
}

void interpretarNodeReturn(ASTNode *node) {
    debug_interprete("NRETURN")
    if (node->left) {
        interprete(node->left); 
        printf("[INTERPRETE]: %d\n", node->left->valorExp);
    }
    // si no es un return ; (sin nada)
}