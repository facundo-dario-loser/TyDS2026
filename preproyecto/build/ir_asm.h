#ifndef IR_ASM_H
#define IR_ASM_H

#include <stdio.h>
#include <string.h>
#include "../TADs/ast.h"
#include "../TADs/ts.h"

// pseudo assembly usando codigo de 3 direcciones

// TODO: ponerle el prefijo IR_ a cada elem del enum
typedef enum InstructionType {
    ADD,
    MULTIPLICATION,
    AND,
    OR,
    ASSIGNMENT,
    RET,
} InstructionType;

typedef struct Instruction {
    InstructionType type;
    Symbol          *op1;
    Symbol          *op2;
    Symbol          *result;
} Instruction;

void generarPseudoAsm(ASTNode *root);

#endif // ASM_H