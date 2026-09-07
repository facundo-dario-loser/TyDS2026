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
    INSTRUCTION_BEGIN_FUNCTION,
    INSTRUCTION_END_FUNCTION
} InstructionType;

typedef struct Instruction {
    InstructionType    type;
    Symbol             *op1;
    Symbol             *op2;
    Symbol             *result;
    struct Instruction *next; // lista doblemente enlazada
    struct Instruction *prev;
} Instruction;

// inserta la instruccion 'i'a la cabeza y retorna la nueva cabeza (que es 'i')
Instruction * insertarInstruction(Instruction **pHead, Instruction *i);

// genera el pseudo assembly y construye una lista enlazada en memoria
Instruction * generarPseudoAsmList(ASTNode *root);

void printInstruction(Instruction *i);

// printea toda la lista de instrucciones
void printInstructions(Instruction *head);

void writeInstruction(Instruction *i, FILE *f);

// escribe toda la lista de isntrucciones en el archivo '3dir.ir'
void writeInstructions(Instruction *head);

void freeInstructionList(Instruction *head);

#endif // ASM_H