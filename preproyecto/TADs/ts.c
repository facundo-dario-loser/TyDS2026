#include "ts.h"

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

    // no deberia borrar los simbolos de este nivel porque estan apuntados por nodos del ast
    //Symbol *aux = oldTope->head;

    //while (aux != NULL) {
    //    Symbol *nextSymbol = aux->next;
    //    free(aux);
    //    aux = nextSymbol;
    //}

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

void printTS(TS *ts) {
    printf("****TS****\n");
    Level *levelAux = ts->tope;
    Symbol *symbolAux;

    while (levelAux != NULL) {
        symbolAux = levelAux->head;

        while (symbolAux != NULL) {
            printf("[%s] -> ", symbolAux->nombre);
            symbolAux = symbolAux->next;
        }
        printf("NULL\n");
        printf("ʌ\n");
        printf("|\n");
        levelAux = levelAux->next;
    }
}

void freeSymbol(Symbol *s) {
    if (s->parametros) {
        Symbol *aux = s->parametros;

        while (aux != NULL) {
            freeSymbol(aux);
            aux = aux->next;
        }
    }

    // creo que no deberia liberar la lista entera. los otros simbolos estan apuntados por nodos del ast
    //if (s->next) {
    //    freeSymbol(s->next);
    //}

    free(s);
}
