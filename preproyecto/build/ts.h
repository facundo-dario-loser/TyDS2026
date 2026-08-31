#ifndef TS_H
#define TS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// tabla de simbolos (implementada como una pila de niveles en donde cada nivel es una lista enlazada)

typedef enum Flag {
    S_VARIABLE,
    S_FUNCION,
    S_PARAMETRO,
} Flag;

typedef enum SemanticType {
    S_INT,
    S_BOOL,
    S_VOID
} SemanticType;

typedef struct Symbol { // lista enlazada de simbolos para un nivel
    Flag          flag;
    char          *nombre;
    SemanticType  tipo;
    int           valor; // para int's y bool's (se usa en el interprete)
    struct Symbol *parametros;
    struct Symbol *next;
} Symbol;

typedef struct SymbolConfig { // sirve para rellenar los campos que se deseen al insertar un nuevo simbolo
    Flag          flag;
    char          *nombre;
    SemanticType  tipo;
    int           valor;
    struct Symbol *parametros;
} SymbolConfig;

typedef struct Level {
    Symbol       *head;
    struct Level *next;
} Level;

typedef struct TS { // pila de niveles (implementada usando una lista enlazada)
    Level *tope;
} TS;

void initTS(TS *ts);
void abrirNivel(TS *ts);
void cerrarNivel(TS *ts);
bool insertarSimbolo(TS *ts, SymbolConfig *config); // lo hace en el nivel del tope. retorna false si la variable a insertar ya fue declarada en el mismo scope
Symbol * buscarSimbolo(TS *ts, char *nombre);       // si no encuentra el simbolo retorna NULL
void printTS(TS *ts);
void freeSymbol(Symbol *s);

// TODO: agregar una funcion para liberar la memoria de la tabla de simbolos

#endif // TS_H