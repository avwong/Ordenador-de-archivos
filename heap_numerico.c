#include "heap.h"
#include <stdio.h>   // fprintf
#include <stdlib.h>  // malloc, free, realloc
#include <string.h>  // strcmp, strlen, memcpy

struct heap_numerico* crear_heap_numerico(int capacidad_inicial) {


};

/*
E: puntero de nodos a intercambiar
S: void
R: las entradas deben ser nodos 
*/
void intercambiar_nodos_numerico(struct nodo_heap_numerico* a, struct nodo_heap_numerico* b) {
    struct nodo_heap_numerico temp = *a;
    *a = *b;
    *b = temp;
}

void 

