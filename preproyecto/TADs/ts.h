#ifndef TS_H
#define TS_H

// Tabla de simbolos (implementada como una pila de niveles en donde cada nivel es una lista enlazada)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef DEBUG_TS 
    #define debug_ts(ts) printTS(ts)
#else
    #define debug_ts(ts)
#endif

typedef enum Flag {
    FLAG_VARIABLE,
    FLAG_FUNCION,
    FLAG_PARAMETRO,
    FLAG_CONSTANTE, // se usa en el generador de pseudo assembly
} Flag;

typedef enum SemanticType {
    SEMANTIC_TYPE_INT,
    SEMANTIC_TYPE_BOOL,
    SEMANTIC_TYPE_VOID
} SemanticType;

typedef struct Symbol {
    Flag          flag;
    char          *nombre;
    SemanticType  tipo;
    int           valor;       // para int's y bool's
    struct Symbol *parametros; // para funciones
    struct Symbol *next;
    int           refCount;    // contador de referencias (de nodos del ast apuntando al simbolo)
} Symbol;

// sirve para rellenar los campos que se deseen al insertar un nuevo simbolo
// de esta forma evito crear muchas funciones o una sola funcion con muchos parametros
typedef struct SymbolConfig {
    Flag          flag;
    char          *nombre;
    SemanticType  tipo;
    int           valor;
    struct Symbol *parametros;
    int           refCount;
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
void freeTS(TS *ts);                                // libera la pila, pero no elimina los simbolos ya que son usados por el interprete

#endif // TS_H