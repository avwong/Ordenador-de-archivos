#include "heap.h"
#include <stdio.h>   // fprintf
#include <stdlib.h>  // malloc, free, realloc
#include <string.h>  // strcmp, strlen, memcpy

struct heap_numerico* crear_heap_numerico(int capacidad_inicial) {


};

/*
E: puntero al heap , indices de los nodos a intercambiar
S: void
R: el heap debe existir, los indices deben ser validos
*/
void intercambiar_nodos_numerico(struct heap_numerico* heap, int i, int j) {
    
    //validaciones
    if (heap == NULL || i < 0 || j < 0 || i >= heap->tamano || j >= heap->tamano) {
        printf("Error: no se pudo intercambiar nodos numericos, indices invalidos.\n");
        return;
    }
    
    struct nodo_heap_numerico temp = heap->nodos[i];
    heap->nodos[i] = heap->nodos[j];
    heap->nodos[j] = temp;
}

/*
Asegura que siempre haya espacio en el heap para agregar un nodo mas en caso de que se llegue a llenar el heap
E: puntero al heap numérico 
S: void
R: que exista el heap, que tenga capacidad mayor a 0
*/
void asegurar_capacidad_numerico(struct heap_numerico* heap) {
    
    //validacion: que exista el heap
    if (heap == NULL){
        return;
    }

    //si aun hay campo, no se ocupa
    if (heap->tamano < heap->capacidad) {
        return;
    }

    //duplica la capacidad del heap
    int nueva_capacidad = heap->capacidad * 2; 
    
    //validacion por si la nueva capacidad llega a ser inutil (no deberia suceder)
    if (nueva_capacidad <= 0){
        nueva_capacidad = 1;
    }

    //expande en donde ya esta
    struct nodo_heap_numerico* nuevo = realloc(heap->nodos, nueva_capacidad * sizeof(struct nodo_heap_numerico));

    if (nuevo == NULL) {
        printf("Error: no se pudo redimensionar heap numerico.\n");
        return;
    }

    //actualiza el heap
    heap->nodos = nuevo;
    heap->capacidad = nueva_capacidad;
}

/* 
Intercambia un nodo hijo con su padre si el nodo hijo es menor que el padre
E: heap y el indice del nodo a mover hacia arriba
S: void
R: que el heap exista, asegurarse que el indice sea valido
*/
void subir_numerico(struct heap_numerico* heap, int hijo) {
    while (hijo > 0) {
        int padre = (hijo - 1) / 2; //formula para encontrar al padre

        //asegurarse que el indice del nodo del papa sea menor que el del hijo
        //si el numero de palabras o año es menor, se intercambian
        if (heap->nodos[hijo].llave < heap->nodos[padre].llave) {
            intercambiar_nodos_numerico(heap, hijo, padre);
            hijo = padre;
        } else {
            break;
        }
    }
};

