#ifndef ANALISIS_SEMANTICO_H
#define ANALISIS_SEMANTICO_H

#include "../TADs/ast.h"
//#define TS_IMPLEMENTATION
#include "../TADs/ts.h"
#include <stdbool.h>
#include <string.h>

bool checkChildrenType(ASTNode *node, SemanticType type); // chequea que ambos hijos del nodo sean del mismo tipo (int o bool)
void analisisSemantico(ASTNode *root);

#endif // ANALISIS_SEMANTICO_H