#ifndef IR_ASM_H
#define IR_ASM_H

#include <stdio.h>
#include <string.h>
#include "../TADs/ast.h"
#include "../TADs/ts.h"

// pseudo assembly usando codigo de 3 direcciones

typedef enum InstructionType {
    INSTRUCTION_ADD,            // sumar int's
    INSTRUCTION_MULTIPLICATION, // multiplicar int's
    INSTRUCTION_AND,            // and logico entre bool's
    INSTRUCTION_OR,             // or logico entre bool's
    INSTRUCTION_ASSIGNMENT,     // asignacion para int's y bool's
    INSTRUCTION_RET,            // return
    INSTRUCTION_BEGIN_FUNCTION, // label para indicar donde comienza la funcion
    INSTRUCTION_END_FUNCTION    // label para indicar donde temrmina la funcion
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