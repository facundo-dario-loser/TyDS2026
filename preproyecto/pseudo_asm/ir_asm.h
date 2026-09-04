#ifndef IR_ASM_H
#define IR_ASM_H

#include <stdio.h>
#include <string.h>
#include "../TADs/ast.h"
#include "../TADs/ts.h"

// pseudo assembly usando codigo de 3 direcciones

typedef enum InstructionType {
    INSTRUCTION_ADD,
    INSTRUCTION_MULTIPLICATION,
    INSTRUCTION_AND,
    INSTRUCTION_OR,
    INSTRUCTION_ASSIGNMENT,
    INSTRUCTION_RET,
} InstructionType;

typedef struct Instruction {
    InstructionType type;
    Symbol          *op1;
    Symbol          *op2;
    Symbol          *result;
} Instruction;

void generarPseudoAsm(ASTNode *root);

#endif // ASM_H