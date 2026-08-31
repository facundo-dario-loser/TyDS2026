#include "interprete.h"

void interpretarNEXPSUMA(ASTNode *node, TS *ts);
void interpretarNEXPMULT(ASTNode *node, TS *ts);
void interpretarNEXPAND(ASTNode *node, TS *ts);
void interpretarNEXPOR(ASTNode *node, TS *ts);
void interpretarNCTEINT(ASTNode *node, TS *ts);
void interpretarNCTEBOOL(ASTNode *node, TS *ts);
void interpretarNID(ASTNode *node, TS *ts);
void interpretarNASSIGN(ASTNode *node, TS *ts);
void interpretarNRETURN(ASTNode *node, TS *ts);

void interpreteAux(ASTNode *root, TS *ts) {
    if (!root) return; // para los hijos NULL

    switch (root->tipo) {
        case N_PROG:     interpreteAux(root->right, ts); break;
        case N_TYPE:                                     break;
        case N_CUERPO:   interpreteAux(root->left, ts);
                         interpreteAux(root->right, ts); break;
        case N_DECL:                                     break;
        case N_EXP_SUMA: interpretarNEXPSUMA(root, ts);  break;
        case N_EXP_MULT: interpretarNEXPMULT(root, ts);  break;
        case N_EXP_AND:  interpretarNEXPAND(root, ts);   break;
        case N_EXP_OR:   interpretarNEXPOR(root, ts);    break;
        case N_CTE_INT:  interpretarNCTEINT(root, ts);   break;
        case N_CTE_BOOL: interpretarNCTEBOOL(root, ts);  break;
        case N_ID:       interpretarNID(root, ts);       break;
        case N_ASSIGN:   interpretarNASSIGN(root, ts);   break;
        case N_RETURN:   interpretarNRETURN(root, ts);   break;
    }
}

void interprete(ASTNode *root) {
    TS ts;
    initTS(&ts);

    if (!root) {
        printf("[ERROR:interprete]: AST vacio\n");
        exit(EXIT_FAILURE);
    }

    interpreteAux(root, &ts);
}

void interpretarNEXPSUMA(ASTNode *node, TS *ts) {
    debug_interprete("NEXPSUMA")
    interpreteAux(node->left, ts);
    interpreteAux(node->right, ts);
    node->valorExp = node->left->valorExp + node->right->valorExp;
    //debugTS(ts);
}

void interpretarNEXPMULT(ASTNode *node, TS *ts) {
    debug_interprete("NEXPMULT")
    interpreteAux(node->left, ts);
    interpreteAux(node->right, ts);
    node->valorExp = node->left->valorExp * node->right->valorExp;
    //debugTS(ts);
}

void interpretarNEXPAND(ASTNode *node, TS *ts) {
    debug_interprete("NEXPAND")
    interpreteAux(node->left, ts);
    interpreteAux(node->right, ts);
    node->valorExp = node->left->valorExp && node->right->valorExp;
    //debugTS(ts);
}

void interpretarNEXPOR(ASTNode *node, TS *ts) {
    debug_interprete("NEXPOR")
    interpreteAux(node->left, ts);
    interpreteAux(node->right, ts);
    node->valorExp = node->left->valorExp || node->right->valorExp;
    //debugTS(ts);
}

void interpretarNCTEINT(ASTNode *node, TS *ts) {
    debug_interprete("NCTEINT")
    node->valorExp = node->valor;
}

void interpretarNCTEBOOL(ASTNode *node, TS *ts) {
    debug_interprete("NCTEBOOL")
    node->valorExp = node->valor;
}

void interpretarNID(ASTNode *node, TS *ts) {
    debug_interprete("NID")
    node->valorExp = node->simbolo->valor;
}

void interpretarNASSIGN(ASTNode *node, TS *ts) {
    debug_interprete("NASSIGN")
    interpreteAux(node->right, ts);
    node->left->simbolo->valor = node->right->valorExp;
}

void interpretarNRETURN(ASTNode *node, TS *ts) {
    debug_interprete("NRETURN")
    if (node->left) {
        interpreteAux(node->left, ts); 
        printf("[INTERPRETE]: %d\n", node->left->valorExp);
    }
    // si no es un return ; (sin nada)
}