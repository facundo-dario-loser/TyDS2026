#include "ir_asm.h"

void generarPseudoAsmListAux(ASTNode *root, Instruction **list, int *temporalesCount);

void generarPseudoAsmNodeProg(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeExpSuma(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeExpMult(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeExpAnd(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeExpOr(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeCteInt(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeCteBool(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeAssign(ASTNode *node, Instruction **list, int *temporalesCount);
void generarPseudoAsmNodeReturn(ASTNode *node, Instruction **list, int *temporalesCount);

void insertarInstruction(Instruction **pHead, Instruction *i) {
    i->prev = NULL;
    i->next = *pHead;

    if (*pHead != NULL) (*pHead)->prev = i;
    
    *pHead = i;    
}

Instruction * generarPseudoAsmList(ASTNode *root) {
    if (!root) {
        printf("[ERROR:IR]: root es NULL\n");
        exit(EXIT_FAILURE);
    }

    // lleva la cuenta de variables temporales para usar con resultados de expresiones
    int temporalesCount = 0;

    Instruction *i = NULL;
    generarPseudoAsmListAux(root, &i, &temporalesCount);
    return i;
}

void generarPseudoAsmListAux(ASTNode *root, Instruction **list, int *temporalesCount) {
    if (!root) return;

    switch (root->tipo) {
        case NODE_PROG:     generarPseudoAsmNodeProg(root, list, temporalesCount);       break;
        case NODE_TYPE:                                                                  break;
        case NODE_CUERPO:   generarPseudoAsmListAux(root->left, list, temporalesCount); 
                            generarPseudoAsmListAux(root->right, list, temporalesCount); break;
        case NODE_DECL:                                                                  break;
        case NODE_EXP_SUMA: generarPseudoAsmNodeExpSuma(root, list, temporalesCount);    break;
        case NODE_EXP_MULT: generarPseudoAsmNodeExpMult(root, list, temporalesCount);    break;
        case NODE_EXP_AND:  generarPseudoAsmNodeExpAnd(root, list, temporalesCount);     break;
        case NODE_EXP_OR:   generarPseudoAsmNodeExpOr(root, list, temporalesCount);      break;
        case NODE_CTE_INT:  generarPseudoAsmNodeCteInt(root, list, temporalesCount);     break;
        case NODE_CTE_BOOL: generarPseudoAsmNodeCteBool(root, list, temporalesCount);    break;
        case NODE_ID:                                                                    break;
        case NODE_ASSIGN:   generarPseudoAsmNodeAssign(root, list, temporalesCount);     break;
        case NODE_RETURN:   generarPseudoAsmNodeReturn(root, list, temporalesCount);     break;
    }
}

void generarPseudoAsmNodeProg(ASTNode *node, Instruction **list, int *temporalesCount) {
    // por ahora solo tenemos la funcion main
    Instruction *i1 = (Instruction*)malloc(sizeof(Instruction));
    i1->type   = INSTRUCTION_BEGIN_FUNCTION;
    i1->op1    = NULL;
    i1->op2    = NULL;
    i1->result = node->simbolo;

    Instruction *i2 = (Instruction*)malloc(sizeof(Instruction));
    i2->type   = INSTRUCTION_END_FUNCTION;
    i2->op1    = NULL;
    i2->op2    = NULL;
    i2->result = node->simbolo;

    insertarInstruction(list, i1);
    generarPseudoAsmListAux(node->right, list, temporalesCount); // instrucciones del cuerpo de la func
    insertarInstruction(list, i2);
}

void generarPseudoAsmNodeExpSuma(ASTNode *node, Instruction **list, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmListAux(node->left, list, temporalesCount);
    generarPseudoAsmListAux(node->right, list, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = FLAG_VARIABLE;
    temp->tipo       = SEMANTIC_TYPE_INT;
    temp->parametros = NULL;
    temp->refCount   = 1; // solo va a ser apuntado por este nodo el temporal

    node->simbolo = temp;

    Instruction *i = (Instruction*)malloc(sizeof(Instruction));
    i->type   = INSTRUCTION_ADD;
    i->op1    = node->left->simbolo;
    i->op2    = node->right->simbolo;
    i->result = temp;
    
    insertarInstruction(list, i);

    (*temporalesCount)++;
}

void generarPseudoAsmNodeExpMult(ASTNode *node, Instruction **list, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmListAux(node->left, list, temporalesCount);
    generarPseudoAsmListAux(node->right, list, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = FLAG_VARIABLE;
    temp->tipo       = SEMANTIC_TYPE_INT;
    temp->parametros = NULL;
    temp->refCount   = 1; // solo va a ser apuntado por este nodo el temporal

    node->simbolo = temp;

    Instruction *i = (Instruction*)malloc(sizeof(Instruction));
    i->type   = INSTRUCTION_MULTIPLICATION;
    i->op1    = node->left->simbolo;
    i->op2    = node->right->simbolo;
    i->result = temp;

    insertarInstruction(list, i);

    (*temporalesCount)++;
}

void generarPseudoAsmNodeExpAnd(ASTNode *node, Instruction **list, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmListAux(node->left, list, temporalesCount);
    generarPseudoAsmListAux(node->right, list, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = FLAG_VARIABLE;
    temp->tipo       = SEMANTIC_TYPE_BOOL;
    temp->parametros = NULL;
    temp->refCount   = 1; // solo va a ser apuntado por este nodo el temporal

    node->simbolo = temp;

    Instruction *i = (Instruction*)malloc(sizeof(Instruction));
    i->type   = INSTRUCTION_AND;
    i->op1    = node->left->simbolo;
    i->op2    = node->right->simbolo;
    i->result = temp;

    insertarInstruction(list, i);

    (*temporalesCount)++;
}

void generarPseudoAsmNodeExpOr(ASTNode *node, Instruction **list, int *temporalesCount) {
    // primero generas las instrucciones para la expresion cada hijo
    generarPseudoAsmListAux(node->left, list, temporalesCount);
    generarPseudoAsmListAux(node->right, list, temporalesCount);
    
    // simbolo para el resultado de la expresion
    char tempNombre[24] = "t";
    char strTempCount[8];

    snprintf(strTempCount, 8, "%d", *temporalesCount);
    strcat(tempNombre, strTempCount);

    Symbol *temp     = (Symbol*)malloc(sizeof(Symbol));
    temp->nombre     = strdup(tempNombre);
    temp->flag       = FLAG_VARIABLE;
    temp->tipo       = SEMANTIC_TYPE_BOOL;
    temp->parametros = NULL;
    temp->refCount   = 1; // solo va a ser apuntado por este nodo el temporal

    node->simbolo = temp;

    Instruction *i = (Instruction*)malloc(sizeof(Instruction));
    i->type   = INSTRUCTION_OR;
    i->op1    = node->left->simbolo;
    i->op2    = node->right->simbolo;
    i->result = temp;

    insertarInstruction(list, i);

    (*temporalesCount)++;
}

void generarPseudoAsmNodeCteInt(ASTNode *node, Instruction **list, int *temporalesCount) {
    char strCteInt[8];
    snprintf(strCteInt, 8, "%d", node->valor);

    Symbol *s     = (Symbol*)malloc(sizeof(Symbol));
    s->flag       = FLAG_CONSTANTE;
    s->nombre     = strdup(strCteInt);
    s->tipo       = SEMANTIC_TYPE_INT;
    s->valor      = node->valor;
    s->parametros = NULL;
    s->refCount   = 1; // solo va a ser apuntado por este nodo el simbolo
    
    node->simbolo = s;
}

void generarPseudoAsmNodeCteBool(ASTNode *node, Instruction **list, int *temporalesCount) {
    char strCteBool[8];
    node->valor ? strcpy(strCteBool, "true") : strcpy(strCteBool, "false");

    Symbol *s     = (Symbol*)malloc(sizeof(Symbol));
    s->flag       = FLAG_CONSTANTE;
    s->nombre     = strdup(strCteBool);
    s->tipo       = SEMANTIC_TYPE_BOOL;
    s->valor      = node->valor;
    s->parametros = NULL;
    s->refCount   = 1; // solo va a ser apuntado por este nodo el simbolo

    node->simbolo = s;
}

void generarPseudoAsmNodeAssign(ASTNode *node, Instruction **list, int *temporalesCount) {
    // primero generas las instrucciones para la expresion del hijo derecho
    generarPseudoAsmListAux(node->right, list, temporalesCount);

    Instruction *i = (Instruction*)malloc(sizeof(Instruction));
    i->type   = INSTRUCTION_ASSIGNMENT;
    i->op1    = node->right->simbolo;
    i->op2    = NULL;
    i->result = node->left->simbolo;

    insertarInstruction(list, i);
}

void generarPseudoAsmNodeReturn(ASTNode *node, Instruction **list, int *temporalesCount) {
    // primero generas las instrucciones para la expresion del hijo derecho
    generarPseudoAsmListAux(node->left, list, temporalesCount);
    
    Instruction a = (Instruction) {
        .type   = INSTRUCTION_RET,
        .op1    = node->left->simbolo,
        .op2    = NULL,
        .result = NULL,
    };

    Instruction *i = (Instruction*)malloc(sizeof(Instruction));
    i->type   = INSTRUCTION_RET;
    i->op1    = node->left->simbolo;
    i->op2    = NULL;
    i->result = NULL;

    insertarInstruction(list, i);
}

void getInstructionStr(Instruction *i, char inst[1024]) {
    char strType[64];
    char strOp1[64];
    char strOp2[64];
    char strResult[64];
    //char inst[1024] = ""; // para guardar el resultado de toda la instruccion

    switch (i->type) {
        case INSTRUCTION_ADD:            strcpy(strType, "ADD");            break;
        case INSTRUCTION_MULTIPLICATION: strcpy(strType, "MULTIPLICATION"); break;
        case INSTRUCTION_AND:            strcpy(strType, "AND");            break;
        case INSTRUCTION_OR:             strcpy(strType, "OR");             break;
        case INSTRUCTION_ASSIGNMENT:     strcpy(strType, "ASSIGNMENT");     break;
        case INSTRUCTION_RET:            strcpy(strType, "RET");            break;
        case INSTRUCTION_BEGIN_FUNCTION: strcpy(strType, "BEGIN_FUNCTION"); break;
        case INSTRUCTION_END_FUNCTION:   strcpy(strType, "END_FUNCTION");   break;
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
}

void printInstruction(Instruction *i) {
    char inst[1024] = "";
    getInstructionStr(i, inst);
    printf("%s", inst);
}

void printInstructions(Instruction *head) {
    Instruction *aux = head;

    // se printea de atras para adelante porque siempre se insertaron las instrucciones a la cabeza
    while (aux->next != NULL) {
        aux = aux->next;
    }

    printf("\n");

    while (aux != NULL) {
        printInstruction(aux);
        aux = aux->prev;
    }

    printf("\n");
}

void writeInstruction(Instruction *i, FILE *f) {
    char inst[1024] = "";
    getInstructionStr(i, inst);
    fprintf(f, "%s", inst);
}

void writeInstructions(Instruction *head) {
    Instruction *aux = head;

     FILE *f = fopen("3dir.ir", "w");

    // se printea de atras para adelante porque siempre se insertaron las instrucciones a la cabeza
    while (aux->next != NULL) {
        aux = aux->next;
    }

    while (aux != NULL) {
        writeInstruction(aux, f);
        aux = aux->prev;
    }

    fclose(f);
}

void freeInstructionList(Instruction *head) {
    // NOTA: no se deben liberar los simbolos a los que apuntan  
    // cada operando por que esos se eliminan al hacer free del ast
    Instruction *aux = head;

    while (aux != NULL) {
        Instruction *next = aux->next;
        free(aux);
        aux = next;
    }
}
