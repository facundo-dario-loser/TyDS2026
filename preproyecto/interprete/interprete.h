#ifndef INTERPRETE_H
#define INTERPRETE_H

#include "../TADs/ast.h"
#include "../TADs/ts.h"

// interprete recursivo

#if DEBUG_INTERPRETE
    #define debug_interprete(...) printf("[INTERPRETE]: %s\n", __VA_ARGS__);
#else
    #define debug_interprete(...)
#endif

void interprete(ASTNode *root);

#endif // INTERPRETE_H