#ifndef TS_H
#define TS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// tabla de simbolos (implementada como una pila de niveles en donde cada nivel es una lista enlazada)

typedef enum Flag {
    VARIABLE,
    FUNCION,
    PARAMETRO,
} Flag;

typedef enum Type {
    INT,
    BOOL,
    VOID
} Type;

typedef struct Symbol { // lista enlazada de simbolos para un nivel
    Flag          flag;
    char          *nombre;
    Type          tipo;
    struct Symbol *parametros;
    struct Symbol *next;
} Symbol;

typedef struct SymbolConfig { // sirve para rellenar los campos que se deseen al insertar un nuevo simbolo
    Flag          flag;
    char          *nombre;
    Type          tipo;
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

#ifdef TS_IMPLEMENTATION

void initTS(TS *ts) {
    ts->tope = NULL;
    abrirNivel(ts);
}

void abrirNivel(TS *ts) {
    Level *l = (Level*)malloc(sizeof(Level)); 
    l->head  = NULL;
    l->next  = ts->tope; 
    ts->tope = l;
}

void cerrarNivel(TS *ts) {
    Level *oldTope = ts->tope;
    ts->tope       = ts->tope->next;

    // primero liberar todos los simbolos del nivel
    Symbol *aux = oldTope->head;

    while (aux != NULL) {
        Symbol *nextSymbol = aux->next;
        free(aux);
        aux = nextSymbol;
    }

    // luego liberar el nivel
    free(oldTope);
}

bool insertarSimbolo(TS *ts, SymbolConfig *config) {
    // primero chequea que no exista el simbolo en el nivel corriente
    Symbol *aux = ts->tope->head;

    while (aux != NULL) {
        if (strcmp(aux->nombre, config->nombre) == 0) {
            return false; // la variable ya fue declarada
        }
        aux = aux->next;
    }

    Symbol *s     = (Symbol*)malloc(sizeof(Symbol));
    s->flag       = config->flag;
    s->nombre     = strdup(config->nombre);
    s->tipo       = config->tipo;
    s->parametros = config->parametros;
    s->next       = ts->tope->head;

    // inserta siempre a la cabeza
    ts->tope->head = s;
    return true;
}

Symbol * buscarSimbolo(TS *ts, char *nombre) {
     Level *nivelActual = ts->tope;
    
    // recorremos el nivel del tope al principio
    while (nivelActual != NULL) {
        Symbol *aux = nivelActual->head;
        
        // recorre la lista de simbolos del nivel corriente
        while (aux != NULL) {
            if (strcmp(aux->nombre, nombre) == 0) return aux;
            aux = aux->next;
        }
        
        // cambia al nivel de mas afuera
        nivelActual = nivelActual->next;
    }
    
    // la variable no fue declarada
    return NULL; 
}

#endif // TS_IMPLEMENTATION

#endif // TS_H