#include "ir_asm.h"

void generarPseudoAsmAux(ASTNode *root, FILE *f, int *temporalesCount);

void generarPseudoAsmNEXPSUMA(ASTNode *node, FILE *f, int *temporalesCount);
void generarPseudoAsmNEXPMULT(ASTNode *node, FILE *f, int *temporalesCount);
void generarPseudoAsmNEXPAND(ASTNode *node, FILE *f, int *temporalesCount);
void generarPseudoAsmNEXPOR(ASTNode *node, FILE *f, int *temporalesCount);
void generarPseudoAsmNCTEINT(ASTNode *node, FILE *f, int *temporalesCount);
void generarPseudoAsmNCTEBOOL(ASTNode *node, FILE *f, int *temporalesCount);
void generarPseudoAsmNASSIGN(ASTNode *node, FILE *f, int *temporalesCount);
void generarPseudoAsmNRETURN(ASTNode *node, FILE *f, int *temporalesCount);

void escribirInstruccion(Instruction *i, FILE *f);

void generarPseudoAsm(ASTNode *root) {
    if (!root) {
        printf("[ERROR:IR]: root es NULL\n");
        exit(EXIT_FAILURE);
    }

    // lleva la cuenta de variables temporales para usar con resultados de expresiones
    int temporalesCount = 0;

    FILE *f = fopen("3dir.ir", "w");
    generarPseudoAsmAux(root, f, &temporalesCount);
    fclose(f);

    // liberamos la memoria de todos los nodos del arbol (y por ende todos los simbolos creados)
    freeAST(root);

    printf("[LOG]: generacion de pseudo assembly completado\n");
}

void generarPseudoAsmAux(ASTNode *root, FILE *f, int *temporalesCount) {
    if (!root) return;

    switch (root->tipo) {
        case N_PROG:     generarPseudoAsmAux(root->right, f, temporalesCount); break;
        case N_TYPE:     break;
        case N_CUERPO:   generarPseudoAsmAux(root->left, f, temporalesCount); 
                         generarPseudoAsmAux(root->right, f, temporalesCount);        
                         break;
        case N_DECL:     break;
        case N_EXP_SUMA: generarPseudoAsmNEXPSUMA(root, f, temporalesCount); break;
        case N_EXP_MULT: generarPseudoAsmNEXPMULT(root, f, temporalesCount); break;
        case N_EXP_AND:  generarPseudoAsmNEXPAND(root, f, temporalesCount); break;
        case N_EXP_OR:   generarPseudoAsmNEXPOR(root, f, temporalesCount); break;
        case N_CTE_INT:  generarPseudoAsmNCTEINT(root, f, temporalesCount); break;
        case N_CTE_BOOL: generarPseudoAsmNCTEBOOL(root, f, temporalesCount); break;
        case N_ID:       break;
        case N_ASSIGN:   generarPseudoAsmNASSIGN(root, f, temporalesCount); break;
        case N_RETURN:   generarPseudoAsmNRETURN(root, f, temporalesCount); break;
    }
}

void escribirInstruccion(Instruction *i, FILE *f) {
    char strType[64];
    char strOp1[64];
    char strOp2[64];
    char strResult[64];
    char inst[1024] = ""; // para guardar el resultado de toda la instruccion

    switch (i->type) {
        case ADD:            strcpy(strType, "ADD");            break;
        case MULTIPLICATION: strcpy(strType, "MULTIPLICATION"); break;
        case AND:            strcpy(strType, "AND");            break;
        case OR:             strcpy(strType, "OR");             break;
        case ASSIGNMENT:     strcpy(strType, "ASSIGNMENT");     break;
        case RET:            strcpy(strType, "RET");            break;
    }

    i->op1    ? strcpy(strOp1, i->op1->nombre) : strcpy(strOp1, "NULL");
    i->op2    ? strcpy(strOp2, i->op2->nombre) : strcpy(strOp2, "NULL");
    i->result ? strcpy(strResult, i->result->nombre) : strcpy(strResult, "NULL");

    // agregar espacios entre cada parte de la instruccion
    char space[2] = " ";
    strcat(strType, space);
    strcat(strOp1, space);
    strcat(strOp2, space);
    strcat(strResult, space);

    // concatenar todas las partes
    strcat(inst, strType);
    strcat(inst, strOp1);
    strcat(inst, strOp2);
    strcat(inst, strResult);
    strcat(inst, "\n");

    fprintf(f, "%s", inst);
}

void generarPseudoAsmNEXPSUMA(ASTNode *node, FILE *f, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmAux(node->left, f, temporalesCount);
    generarPseudoAsmAux(node->right, f, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = S_VARIABLE;
    temp->tipo       = S_INT;
    temp->parametros = NULL;

    node->simbolo = temp;
    
    Instruction i = (Instruction) {
        .type   = ADD,
        .op1    = node->left->simbolo,
        .op2    = node->right->simbolo,
        .result = temp,
    };

    escribirInstruccion(&i, f);
    (*temporalesCount)++;
}

void generarPseudoAsmNEXPMULT(ASTNode *node, FILE *f, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmAux(node->left, f, temporalesCount);
    generarPseudoAsmAux(node->right, f, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = S_VARIABLE;
    temp->tipo       = S_INT;
    temp->parametros = NULL;

    node->simbolo = temp;
    
    Instruction i = (Instruction) {
        .type   = MULTIPLICATION,
        .op1    = node->left->simbolo,
        .op2    = node->right->simbolo,
        .result = temp,
    };

    escribirInstruccion(&i, f);
    (*temporalesCount)++;
}

void generarPseudoAsmNEXPAND(ASTNode *node, FILE *f, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmAux(node->left, f, temporalesCount);
    generarPseudoAsmAux(node->right, f, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = S_VARIABLE;
    temp->tipo       = S_BOOL;
    temp->parametros = NULL;

    node->simbolo = temp;
    
    Instruction i = (Instruction) {
        .type   = AND,
        .op1    = node->left->simbolo,
        .op2    = node->right->simbolo,
        .result = temp,
    };

    escribirInstruccion(&i, f);
    (*temporalesCount)++;
}

void generarPseudoAsmNEXPOR(ASTNode *node, FILE *f, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmAux(node->left, f, temporalesCount);
    generarPseudoAsmAux(node->right, f, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = S_VARIABLE;
    temp->tipo       = S_BOOL;
    temp->parametros = NULL;

    node->simbolo = temp;
    
    Instruction i = (Instruction) {
        .type   = OR,
        .op1    = node->left->simbolo,
        .op2    = node->right->simbolo,
        .result = temp,
    };

    escribirInstruccion(&i, f);
    (*temporalesCount)++;
}

void generarPseudoAsmNCTEINT(ASTNode *node, FILE *f, int *temporalesCount) {
    char strCteInt[8];
    snprintf(strCteInt, 8, "%d", node->valor);

    Symbol *s     = (Symbol*)malloc(sizeof(Symbol));
    s->flag       = S_CONSTANTE;
    s->nombre     = strdup(strCteInt);
    s->tipo       = S_INT;
    s->valor      = node->valor;
    s->parametros = NULL;
    
    node->simbolo = s;
}

void generarPseudoAsmNCTEBOOL(ASTNode *node, FILE *f, int *temporalesCount) {
    char strCteBool[8];
    node->valor ? strcpy(strCteBool, "true") : strcpy(strCteBool, "false");

    Symbol *s     = (Symbol*)malloc(sizeof(Symbol));
    s->flag       = S_CONSTANTE;
    s->nombre     = strdup(strCteBool);
    s->tipo       = S_BOOL;
    s->valor      = node->valor;
    s->parametros = NULL;

    node->simbolo = s;
}

void generarPseudoAsmNASSIGN(ASTNode *node, FILE *f, int *temporalesCount) {
    // primero generas las instrucciones para la expresion del hijo derecho
    generarPseudoAsmAux(node->right, f, temporalesCount);
    
    Instruction i = (Instruction) {
        .type   = ASSIGNMENT,
        .op1    = node->right->simbolo,
        .op2    = NULL,
        .result = node->left->simbolo,
    };

    escribirInstruccion(&i, f);
}

void generarPseudoAsmNRETURN(ASTNode *node, FILE *f, int *temporalesCount) {
    // primero generas las instrucciones para la expresion del hijo derecho
    generarPseudoAsmAux(node->left, f, temporalesCount);
    
    Instruction i = (Instruction) {
        .type   = RET,
        .op1    = node->left->simbolo,
        .op2    = NULL,
        .result = NULL,
    };

    escribirInstruccion(&i, f);
}
