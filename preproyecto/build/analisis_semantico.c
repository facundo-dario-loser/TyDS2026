#include "analisis_semantico.h"

// chequea que ambos hijos del nodo sean del mismo tipo (int o bool)
bool checkChildrenType(ASTNode *node, SemanticType type) {
    if (!node->left || !node->right) return false;
    return (node->left->semanticType == type) && (node->right->semanticType == type);
}

// funciones para hacer el analisis semantico para cada tipo de nodo del AST
void analisisNPROG(ASTNode *node, TS *ts);
void analisisNCUERPO(ASTNode *node, TS *ts);
void analisisDECL(ASTNode *node, TS *ts);
void analisisNEXPSUMA(ASTNode *node, TS *ts);
void analisisNEXPMULT(ASTNode *node, TS *ts);
void analisisNEXPAND(ASTNode *node, TS *ts);
void analisisNEXPOR(ASTNode *node, TS *ts);
void analisisNID(ASTNode *node, TS *ts);
void analisisNASSIGN(ASTNode *node, TS *ts);
void analisisNRETURN(ASTNode *node, TS *ts);

void analisisSemanticoAux(ASTNode *root, TS *ts);
void analisisSemantico(ASTNode *root);


void analisisSemanticoAux(ASTNode *root, TS *ts) {
    if (!root) {
        printf("[ERROR:AS]: AST vacio\n");
        exit(EXIT_FAILURE);
    }

    switch (root->tipo) {
        case N_PROG:     analisisNPROG(root, ts);    break;
        case N_TYPE:                                 break;
        case N_CUERPO:   analisisNCUERPO(root, ts);  break;
        case N_DECL:     analisisDECL(root, ts);     break;
        case N_EXP_SUMA: analisisNEXPSUMA(root, ts); break;
        case N_EXP_MULT: analisisNEXPMULT(root, ts); break;
        case N_EXP_AND:  analisisNEXPAND(root, ts);  break;
        case N_EXP_OR:   analisisNEXPOR(root, ts);   break;
        case N_CTE_INT:                              break;
        case N_CTE_BOOL:                             break;
        case N_ID:       analisisNID(root, ts);      break;
        case N_ASSIGN:   analisisNASSIGN(root, ts);  break;
        case N_RETURN:   analisisNRETURN(root, ts);  break;
    }
}

void analisisSemantico(ASTNode *root) {
   TS ts;
   initTS(&ts);
   analisisSemanticoAux(root, &ts);
}

void analisisNPROG(ASTNode *node, TS *ts) {
    // por ahora solo tenemos la funcion main y nos metemos solo 1 nivel adentro 
    SymbolConfig config;
    config.flag       = S_FUNCION;
    config.nombre     = "main";                   // por ahora solo tenemos la funcion main
    config.tipo       = node->left->semanticType; // el tipo que retorna main
    config.parametros = NULL;                     // main no tiene parametros por ahora
    
    // TODO: verificar que no intente crear una funcion dentro de otra
    insertarSimbolo(ts, &config);                       
    abrirNivel(ts); // nos metemos 1 nivel adentro (adentro de main)
    
    node->simbolo = buscarSimbolo(ts, config.nombre);

    if (node->right) { // analizar el cuerpo de la funcion
        analisisSemanticoAux(node->right, ts);
        node->tieneReturn = node->right->tieneReturn;
    }

    if ((config.tipo != S_VOID) && !node->tieneReturn) {
        char *valRet = (config.tipo == S_INT) ? "int" : "bool";
        printf("[ERROR:AS]: main retorna '%s', pero en el cuerpo no se esta retornando nada\n", valRet);
        exit(EXIT_FAILURE);
    }        
}

void analisisNCUERPO(ASTNode *node, TS *ts) {
    analisisSemanticoAux(node->left, ts);
    if (node->right) analisisSemanticoAux(node->right, ts);
    
    bool leftTieneReturn  = false;
    bool rightTieneReturn = false;
    if (node->left)  leftTieneReturn  = node->left->tieneReturn;
    if (node->right) rightTieneReturn = node->right->tieneReturn;
    node->tieneReturn = leftTieneReturn || rightTieneReturn;
}

void analisisDECL(ASTNode *node, TS *ts) {
    SymbolConfig config;
    config.flag = S_VARIABLE;
    config.tipo = node->left->semanticType;
    if (node->right->nombre) config.nombre = strdup(node->right->nombre);

    if (config.tipo == S_VOID) {
        printf("[ERROR:AS]: no se pueden declarar variables void -> '%s' (linea: %d)\n", config.nombre, node->line);
        exit(EXIT_FAILURE);
    }

    bool res = insertarSimbolo(ts, &config);
    if (!res) {
        printf("[ERROR:AS]: variable redeclarada -> '%s' (linea: %d)\n", config.nombre, node->line);
        exit(EXIT_FAILURE);
    }

    node->tieneReturn = false;
}

void analisisNEXPSUMA(ASTNode *node, TS *ts) {
    analisisSemanticoAux(node->left, ts);
    analisisSemanticoAux(node->right, ts);
    
    if (!checkChildrenType(node, S_INT)) { // la suma esta definida solo para ints
        printf("[ERROR:AS]: '+' solo esta definida para operandos de tipo int (linea: %d)\n", node->line);
        
        if (node->left) {
            char *leftOp = (node->left->semanticType == S_INT) ? "int" : "bool";
            printf("\t\tleftOp: %s\n", leftOp);
        } else {
            printf("\t\tleftOp: empty\n");
        }

        if (node->right) {
            char *rightOp = (node->right->semanticType == S_INT) ? "int" : "bool"; 
            printf("\t\trightOp: %s\n", rightOp);
        } else {
            printf("\t\trightOp: empty\n");
        }

        exit(EXIT_FAILURE);                            
    }
    node->semanticType = S_INT;
    node->tieneReturn = false;
}

void analisisNEXPMULT(ASTNode *node, TS *ts) {
    analisisSemanticoAux(node->left, ts);
    analisisSemanticoAux(node->right, ts);
    
    if (!checkChildrenType(node, S_INT)) { // la suma esta definida solo para ints
        printf("[ERROR:AS]: '*' solo esta definida para operandos de tipo int (linea: %d)\n", node->line);
        
        if (node->left) {
            char *leftOp = (node->left->semanticType == S_INT) ? "int" : "bool";
            printf("\t\tleftOp: %s\n", leftOp);
        } else {
            printf("\t\tleftOp: empty\n");
        }

        if (node->right) {
            char *rightOp = (node->right->semanticType == S_INT) ? "int" : "bool"; 
            printf("\t\trightOp: %s\n", rightOp);
        } else {
            printf("\t\trightOp: empty\n");
        }

        exit(EXIT_FAILURE);                            
    }
    node->semanticType = S_INT;
    node->tieneReturn = false;
}

void analisisNEXPAND(ASTNode *node, TS *ts) {
    analisisSemanticoAux(node->left, ts);
    analisisSemanticoAux(node->right, ts);
    
    if (!checkChildrenType(node, S_BOOL)) { // la suma esta definida solo para ints
        printf("[ERROR:AS]: '&&' solo esta definida para operandos de tipo bool (linea: %d)\n", node->line);
        
        if (node->left) {
            char *leftOp = (node->left->semanticType == S_INT) ? "int" : "bool";
            printf("\t\tleftOp: %s\n", leftOp);
        } else {
            printf("\t\tleftOp: empty\n");
        }

        if (node->right) {
            char *rightOp = (node->right->semanticType == S_INT) ? "int" : "bool"; 
            printf("\t\trightOp: %s\n", rightOp);
        } else {
            printf("\t\trightOp: empty\n");
        }

        exit(EXIT_FAILURE);                            
    }
    node->semanticType = S_BOOL;
    node->tieneReturn  = false;
}

void analisisNEXPOR(ASTNode *node, TS *ts) {
    analisisSemanticoAux(node->left, ts);
    analisisSemanticoAux(node->right, ts);
    
    if (!checkChildrenType(node, S_BOOL)) { // la suma esta definida solo para ints
        printf("[ERROR:AS]: '||' solo esta definida para operandos de tipo bool (linea: %d)\n", node->line);
        
        if (node->left) {
            char *leftOp = (node->left->semanticType == S_INT) ? "int" : "bool";
            printf("\t\tleftOp: %s\n", leftOp);
        } else {
            printf("\t\tleftOp: empty\n");
        }

        if (node->right) {
            char *rightOp = (node->right->semanticType == S_INT) ? "int" : "bool"; 
            printf("\t\trightOp: %s\n", rightOp);
        } else {
            printf("\t\trightOp: empty\n");
        }
        
        exit(EXIT_FAILURE);                            
    }
    node->semanticType = S_BOOL;
    node->tieneReturn  = false;
}

void analisisNID(ASTNode *node, TS *ts) {
    Symbol *s = buscarSimbolo(ts, node->nombre);
    
    if (!s) { // TODO agregar linea al msj
        printf("[ERROR:AS]: variable no declarada -> '%s'\n", node->nombre);
        exit(EXIT_FAILURE);
    } else {
        node->semanticType = s->tipo; // lo dejo apuntando para el interprete
        node->simbolo = s;
    }
    
    node->tieneReturn = false;
}

void analisisNASSIGN(ASTNode *node, TS *ts) {
    analisisSemanticoAux(node->left, ts);
    analisisSemanticoAux(node->right, ts);
    
    if (!checkChildrenType(node, node->left->semanticType)) { // la parte derecha de la asignacion deberia tener el mismo tipo que la parte izquierda
        printf("[ERROR:AS]: '=' tipo de leftOp es diferente del tipo de rightOp (linea: %d)\n", node->line);

        if (node->left) {
            char *leftOp = (node->left->semanticType == S_INT) ? "int" : "bool";
            printf("\t\tleftOp: %s\n", leftOp);
        } else {
            printf("\t\tleftOp: empty\n");
        }

        if (node->right) {
            char *rightOp = (node->right->semanticType == S_INT) ? "int" : "bool"; 
            printf("\t\trightOp: %s\n", rightOp);
        } else {
            printf("\t\trightOp: empty\n");
        }

        exit(EXIT_FAILURE);
    }
    node->tieneReturn = false;
}

void analisisNRETURN(ASTNode *node, TS *ts) {
    // hay que chequear que se retorne lo mismo que en el perfil de main
    Symbol *s = buscarSimbolo(ts, "main");
    ASTNode *exp = node->left; // la exp que retorna 'return'
    
    if (exp == NULL) { // si hace 'return;'
        if (s->tipo != S_VOID) {
            char *retType = (s->tipo == S_INT) ? "int" : "bool";
            printf("[ERROR:AS]: main retorna '%s', pero en la funcion se hace return sin nada\n", retType);
            exit(EXIT_FAILURE);
        }
    } else { // si retorna algo 'return exp;'
        if (s->tipo == S_VOID) {
            char *retType = (exp->semanticType == S_INT) ? "int" : "bool";
            printf("[ERROR:AS]: main no retorna nada (void), pero se esta retornando un '%s'\n", retType);
            exit(EXIT_FAILURE);
        }

        analisisSemanticoAux(node->left, ts); // analizar la exp que se retorna

        if ((s->tipo == S_INT) && !(exp->semanticType == S_INT)) {
            char *expType = (exp->semanticType == S_INT) ? "int" : "bool";
            printf("[ERROR:AS]: main retorna un int, pero se esta retornando un '%s'\n", expType);
            exit(EXIT_FAILURE);
        }

        if ((s->tipo == S_BOOL) && !(exp->semanticType == S_BOOL)) {
            char *expType = (exp->semanticType == S_INT) ? "int" : "bool";
            printf("[ERROR:AS]: main retorna un bool, pero se esta retornando un '%s'\n", expType);
            exit(EXIT_FAILURE);
        }
    }
    cerrarNivel(ts); // en el futuro no necesariamente deberia cerrar el nivel (si esta dentro de un if por ej)
    node->tieneReturn = true;
}
