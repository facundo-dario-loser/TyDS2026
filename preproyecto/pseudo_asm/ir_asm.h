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
    INSTRUCTION_END_FUNCTION    // label para indicar donde termina la funcion
} InstructionType;

typedef struct Instruction {
    InstructionType    type;
    Symbol             *op1;
    Symbol             *op2;
    Symbol             *result;
    struct Instruction *next; // lista doblemente enlazada
    struct Instruction *prev;
} Instruction;

// inserta la instruccion 'i' a la cabeza
void insertarInstruction(Instruction **pHead, Instruction *i);

// genera el pseudo assembly y construye una lista enlazada en memoria
// las instrucciones se insertan siempre a la cabeza, por lo que la lista
// esta al revez
Instruction * generarPseudoAsmList(ASTNode *root);

// printea una sola instruccion
void printInstruction(Instruction *i);

// printea toda la lista de instrucciones
void printInstructions(Instruction *head);

// escribe una sola instruccion en un archivo
void writeInstruction(Instruction *i, FILE *f);

// escribe toda la lista de instrucciones en el archivo '3dir.ir'
void writeInstructions(Instruction *head);

// libera la memoria de toda la lista
void freeInstructionList(Instruction *head);

#endif // ASM_H