#include "analisis_semantico.h"

bool checkChildrenType(ASTNode *node, SemanticType type) {
    if (!node->left || !node->right) return false;
    return (node->left->semanticType == type) && (node->right->semanticType == type);
}

void analisisSemanticoAux(ASTNode *root, TS *ts) {
    if (!root) {
        printf("[ERROR:AS]: AST vacio\n");
        exit(EXIT_FAILURE);
    }

    switch (root->tipo) {
        case N_PROG:    {   // por ahora solo tenemos la funcion main y nos metemos solo 1 nivel adentro 
                            SymbolConfig config;
                            config.flag       = S_FUNCION;
                            config.nombre     = "main";                   // por ahora solo tenemos la funcion main
                            config.tipo       = root->left->semanticType; // el tipo que retorna main
                            config.parametros = NULL;                     // main no tiene parametros por ahora
                            abrirNivel(ts);                              // nos metemos 1 nivel adentro (por main)
                            
                            // TODO: verificar que no intente crear una funcion dentro de otra
                            insertarSimbolo(ts, &config);                       
                            
                            if (root->right) { // analizar el cuerpo de la funcion
                                analisisSemanticoAux(root->right, ts);
                                root->tieneReturn = root->right->tieneReturn;
                            }

                            if ((config.tipo != S_VOID) && !root->tieneReturn) {
                                printf("[ERROR:AS]: main retorna algo, pero en el cuerpo no se esta retornando nada\n");
                                exit(EXIT_FAILURE);
                            }        
                         } break;

        case N_TYPE:     break;

        case N_CUERPO:   { analisisSemanticoAux(root->left, ts);
                           if (root->right) analisisSemanticoAux(root->right, ts);
                           
                           bool leftTieneReturn  = false;
                           bool rightTieneReturn = false;
                           if (root->left)  leftTieneReturn  = root->left->tieneReturn;
                           if (root->right) rightTieneReturn = root->right->tieneReturn;
                           root->tieneReturn = leftTieneReturn || rightTieneReturn;
                        } break;

        case N_DECL:     { SymbolConfig config;
                           config.flag = S_VARIABLE;
                           config.tipo = root->left->semanticType;
                           if (root->right->nombre) config.nombre = strdup(root->right->nombre);
                           bool res = insertarSimbolo(ts, &config);
                           if (!res) {
                                printf("[ERROR:AS]: variable re-declarada -> %s\n", config.nombre);
                                exit(EXIT_FAILURE);
                           }
                           root->tieneReturn = false;
                         } break;

        case N_EXP_SUMA: { analisisSemanticoAux(root->left, ts);
                           analisisSemanticoAux(root->right, ts);
                           if (!checkChildrenType(root, S_INT)) { // la suma esta definida solo para ints
                                printf("[ERROR:AS]: '+' solo esta definida para operandos de tipo int\n");
                                exit(EXIT_FAILURE);                            
                           }
                           root->semanticType = S_INT;
                           root->tieneReturn = false;
                         } break;

        case N_EXP_MULT: { analisisSemanticoAux(root->left, ts);
                           analisisSemanticoAux(root->right, ts);
                           if (!checkChildrenType(root, S_INT)) { // la suma esta definida solo para ints
                                printf("[ERROR:AS]: '*' solo esta definida para operandos de tipo int\n");
                                exit(EXIT_FAILURE);                            
                           }
                           root->semanticType = S_INT;
                           root->tieneReturn = false;
                         } break;

        case N_EXP_AND:  { analisisSemanticoAux(root->left, ts);
                           analisisSemanticoAux(root->right, ts);
                           if (!checkChildrenType(root, S_BOOL)) { // la suma esta definida solo para ints
                                printf("[ERROR:AS]: '&&' solo esta definida para operandos de tipo bool\n");
                                exit(EXIT_FAILURE);                            
                           }
                           root->semanticType = S_BOOL;
                           root->tieneReturn = false;
                         } break;

        case N_EXP_OR:  { analisisSemanticoAux(root->left, ts);
                          analisisSemanticoAux(root->right, ts);
                          if (!checkChildrenType(root, S_BOOL)) { // la suma esta definida solo para ints
                                printf("[ERROR:AS]: '||' solo esta definida para operandos de tipo bool\n");
                                exit(EXIT_FAILURE);                            
                          }
                          root->semanticType = S_BOOL;
                          root->tieneReturn = false;
                        } break;

        case N_CTE_INT:  break;
        case N_CTE_BOOL: break;

        case N_ID:       { Symbol *s = buscarSimbolo(ts, root->nombre);
                           if (!s) {
                               printf("[ERROR:AS]: variable no declarada -> %s\n", root->nombre);
                               exit(EXIT_FAILURE);
                           } else {
                               root->semanticType = s->tipo;
                           }
                           root->tieneReturn = false;
                         } break;
        
        case N_ASSIGN:   { analisisSemanticoAux(root->left, ts);
                           analisisSemanticoAux(root->right, ts);
                           if (!checkChildrenType(root, root->left->semanticType)) { // la parte derecha de la asignacion deberia tener el mismo tipo que la parte izquierda
                              printf("[ERROR:AS]: '=' tipo de leftOp es diferente del tipo de rightOp\n");
                              exit(EXIT_FAILURE);
                            }
                            root->tieneReturn = false;
                        } break;

        case N_RETURN:   { // hay que chequear que se retorne lo mismo que en el perfil de main
                           Symbol *s = buscarSimbolo(ts, "main");
                           ASTNode *exp = root->left; // la exp que retorna 'return'
                           
                           if (exp == NULL) { // si hace 'return;'
                                if (s->tipo != S_VOID) {
                                    printf("[ERROR:AS]: main retorna algo, pero en la funcion se hace return sin nada\n");
                                    exit(EXIT_FAILURE);
                                }
                           } else { // si retorna algo 'return exp;'
                                if (s->tipo == S_VOID) {
                                    printf("[ERROR:AS]: main no retorna nada (void), pero se esta retornando una exp\n");
                                    exit(EXIT_FAILURE);
                                }

                                analisisSemanticoAux(root->left, ts); // analizar la exp que se retorna

                                if ((s->tipo == S_INT) && !(exp->semanticType == S_INT)) {
                                    printf("[ERROR:AS]: main retorna un int, pero se esta retornando otro tipo\n");
                                    exit(EXIT_FAILURE);
                                }

                                if ((s->tipo == S_BOOL) && !(exp->semanticType == S_BOOL)) {
                                    printf("[ERROR:AS]: main retorna un bool, pero se esta retornando otro tipo\n");
                                    exit(EXIT_FAILURE);
                                }
                           }
                           cerrarNivel(ts); 
                           root->tieneReturn = true;
                         } break;
    }
}

void analisisSemantico(ASTNode *root) {
   TS ts;
   initTS(&ts);
   analisisSemanticoAux(root, &ts);
}
