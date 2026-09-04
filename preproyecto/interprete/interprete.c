#include "interprete.h"

void interpretarNEXPSUMA(ASTNode *node);
void interpretarNEXPMULT(ASTNode *node);
void interpretarNEXPAND(ASTNode *node);
void interpretarNEXPOR(ASTNode *node);
void interpretarNCTEINT(ASTNode *node);
void interpretarNCTEBOOL(ASTNode *node);
void interpretarNID(ASTNode *node);
void interpretarNASSIGN(ASTNode *node);
void interpretarNRETURN(ASTNode *node);

void interprete(ASTNode *root) {
    if (!root) return; // para los hijos NULL

    switch (root->tipo) {
        case NODE_PROG:     interprete(root->right);    break;
        case NODE_TYPE:                                 break;
        case NODE_CUERPO:   interprete(root->left);
                         interprete(root->right);    break;
        case NODE_DECL:                                 break;
        case NODE_EXP_SUMA: interpretarNEXPSUMA(root);  break;
        case NODE_EXP_MULT: interpretarNEXPMULT(root);  break;
        case NODE_EXP_AND:  interpretarNEXPAND(root);   break;
        case NODE_EXP_OR:   interpretarNEXPOR(root);    break;
        case NODE_CTE_INT:  interpretarNCTEINT(root);   break;
        case NODE_CTE_BOOL: interpretarNCTEBOOL(root);  break;
        case NODE_ID:       interpretarNID(root);       break;
        case NODE_ASSIGN:   interpretarNASSIGN(root);   break;
        case NODE_RETURN:   interpretarNRETURN(root);   break;
    }
}

void interpretarNEXPSUMA(ASTNode *node) {
    debug_interprete("NEXPSUMA")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp + node->right->valorExp;
}

void interpretarNEXPMULT(ASTNode *node) {
    debug_interprete("NEXPMULT")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp * node->right->valorExp;
}

void interpretarNEXPAND(ASTNode *node) {
    debug_interprete("NEXPAND")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp && node->right->valorExp;
}

void interpretarNEXPOR(ASTNode *node) {
    debug_interprete("NEXPOR")
    interprete(node->left);
    interprete(node->right);
    node->valorExp = node->left->valorExp || node->right->valorExp;
}

void interpretarNCTEINT(ASTNode *node) {
    debug_interprete("NCTEINT")
    node->valorExp = node->valor;
}

void interpretarNCTEBOOL(ASTNode *node) {
    debug_interprete("NCTEBOOL")
    node->valorExp = node->valor;
}

void interpretarNID(ASTNode *node) {
    debug_interprete("NID")
    node->valorExp = node->simbolo->valor;
}

void interpretarNASSIGN(ASTNode *node) {
    debug_interprete("NASSIGN")
    interprete(node->right);
    node->left->simbolo->valor = node->right->valorExp;
}

void interpretarNRETURN(ASTNode *node) {
    debug_interprete("NRETURN")
    if (node->left) {
        interprete(node->left); 
        printf("[INTERPRETE]: %d\n", node->left->valorExp);
    }
    // si no es un return ; (sin nada)
}