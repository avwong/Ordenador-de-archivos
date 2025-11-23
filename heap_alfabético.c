#include "heap.h"
#include <stdio.h>   // fprintf
#include <stdlib.h>  // malloc, free, realloc
#include <string.h>  // strcmp, strlen, memcpy



//helper local para copias la llave alfabética
static char* copiar_llave_alfabetica(const char* llave) {
    if (llave == NULL) return NULL;
    size_t len = strlen(llave) + 1; // +1 para el carácter nulo
    char* copia = (char*) calloc(1, len); // Asigna memoria en el heap para la copia
    if (copia != NULL) {
        memcpy(copia, llave, len); // Copia el contenido de la cadena original a la nueva ubicación
    }
    return copia;
}

//funciones del heap alfabético

/*función que intercamia dos nodos del heap alfabético
E: punteros a los nodos a intercambiar
*/  
static void intercambiar_nodos_alfabetico(struct nodo_heap_alfabetico* a    , struct nodo_heap_alfabetico* b) {
    struct nodo_heap_alfabetico temp = *a;
    *a = *b;
    *b = temp;
}

/*función que asegura la capacidad del heap alfabético
E: puntero al heap alfabético   
*/      
static void asegurar_capacidad_alfabetico(struct heap_alfabetico* heap) {
    if (heap->tamano < heap->capacidad) return;

    int nueva_capacidad = heap->capacidad * 2;
    if (nueva_capacidad <= 0){
        fprintf(stderr, "Advertencia: capacidad del heap alfabetico excede el limite.\n");
        nueva_capacidad = 1;
    }
    struct nodo_heap_alfabetico* nuevo =
        (struct nodo_heap_alfabetico*) realloc(
            heap->nodos,
            nueva_capacidad * sizeof(struct nodo_heap_alfabetico)
        );

    if (nuevo == NULL) {
        fprintf(stderr, "Error: no se pudo redimensionar heap alfabetico.\n");
        return;
    }

    heap->nodos = nuevo;
    heap->capacidad = nueva_capacidad;
}

//heapify hacia arriba 
static void subir_alfabetico(struct heap_alfabetico* heap, int idx) {
    while (idx > 0) {
        int padre = (idx - 1) / 2;
        if (strcmp(heap->nodos[idx].llave, heap->nodos[padre].llave) < 0) {
            intercambiar_nodos_alfabetico(&heap->nodos[idx], &heap->nodos[padre]);
            idx = padre;
        } else {
            break;
        }
    }
}
//heapify hacia abajo
static void bajar_alfabetico(struct heap_alfabetico* heap, int idx) {
    int izquierda, derecha, menor;

    while (1) {
        izquierda = 2 * idx + 1;
        derecha = 2 * idx + 2;
        menor = idx;

        if (izquierda < heap->tamano &&
            strcmp(heap->nodos[izquierda].llave, heap->nodos[menor].llave) < 0) {
            menor = izquierda;
        }
        if (derecha < heap->tamano &&
            strcmp(heap->nodos[derecha].llave, heap->nodos[menor].llave) < 0) {
            menor = derecha;
        }
        if (menor != idx) {
            intercambiar_nodos_alfabetico(&heap->nodos[idx], &heap->nodos[menor]);
            idx = menor;
        } else {
            break;
        }
    }
}

/*funcion que crea un heap alfabético
E: capacidad inicial, tamaño inicial del arreglo de nodos
S: puntero al heap alfabético creado, null si falla 
*/
struct heap_alfabetico* crear_heap_alfabetico(int capacidad_inicial) {
    struct heap_alfabetico* heap = (struct heap_alfabetico*) calloc(1, sizeof(struct heap_alfabetico));
    if (heap == NULL) {
        fprintf(stderr, "Error: no se pudo crear el heap alfabetico.\n");
        return NULL;
    }

    heap->nodos = (struct nodo_heap_alfabetico*) calloc(capacidad_inicial, sizeof(struct nodo_heap_alfabetico));
    if (heap->nodos == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para los nodos del heap alfabetico.\n");
        free(heap);
        return NULL;
    }

    heap->tamano = 0;
    heap->capacidad = capacidad_inicial;
    return heap;
}


int heap_alfabetico_vacio(struct heap_alfabetico* heap) {
    return (heap == NULL || heap->tamano == 0);
}

void insertar_heap_alfabetico(struct heap_alfabetico *heap,
                              struct articulo articulo,
                              const char *llave) {
    if (heap == NULL || llave == NULL) return;

    asegurar_capacidad_alfabetico(heap);

    int idx = heap->tamano;

    // Copiamos el articulo por valor (NO liberamos aquí nunca)
    heap->nodos[idx].articulo = articulo;

    // Guardamos copia de la llave
    heap->nodos[idx].llave = copiar_llave_alfabetica(llave);

    heap->tamano++;

    subir_alfabetico(heap, idx);
}
/* Funcion que se encarga de extraer el artículo con la llave alfabética mínima del heap
E: puntero al heap alfabético
S: artículo con la llave alfabética mínima
 */
struct articulo extraer_min_heap_alfabetico(struct heap_alfabetico* heap) {
    // Artículo vacío por si algo sale mal
    struct articulo vacio = {NULL, NULL, NULL, NULL, 0, NULL};

    if (heap == NULL || heap->tamano == 0) {
        return vacio;
    }

    // se guarda el nodo mínimo para devolverlo
    struct nodo_heap_alfabetico min_nodo = heap->nodos[0];

    // Se reemplaza la raíz con el último nodo y reducimos el tamaño
    heap->tamano--;
    if (heap->tamano > 0) {
        heap->nodos[0] = heap->nodos[heap->tamano];
        bajar_alfabetico(heap, 0);
    }

    // se libera la memoria de la llave del nodo mínimo
    free(min_nodo.llave);

    // No se libera
    return min_nodo.articulo;
}

void destruir_heap_alfabetico(struct heap_alfabetico* heap) {
    if (heap == NULL) return;

    // Solo se libera las llaves y el arreglo de nodos.
    // no liberamos los articulos porque no son de este heap.
    for (int i = 0; i < heap->tamano; i++) {
        free(heap->nodos[i].llave);
    }

    free(heap->nodos);
    free(heap);
}

//Funciones de ordenamiento alfabético

/*
 * Ordena un array de artículos por título alfabéticamente (A-Z)
 * E: articulos - array de artículos
 *    n - cantidad de artículos
 * S: nuevo array con artículos ordenados por título
 */
struct articulo* ordenar_por_titulo(struct articulo* articulos, int n) {
    //Validar
    if (articulos == NULL || n <= 0) {
        return NULL;
    }

    // Crear heap alfabético
    struct heap_alfabetico* heap = crear_heap_alfabetico(n); // capacidad inicial = n
    if (heap == NULL) {
        fprintf(stderr, "Error: no se pudo crear heap para ordenar por titulo.\n");
        return NULL;
    }

    // Insertar todos los artículos usando el título como llave
    for (int i = 0; i < n; i++) {
        insertar_heap_alfabetico(heap, articulos[i], articulos[i].titulo_articulo);
    }

    // crea array para artículos ordenados
    struct articulo* ordenados = (struct articulo*) calloc(n, sizeof(struct articulo));
    if (ordenados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para array ordenado.\n");
        destruir_heap_alfabetico(heap);
        return NULL;
    }

    // Extraen artículos del heap que salen ya ordenados alfabéticamente
    for (int i = 0; i < n; i++) {
        ordenados[i] = extraer_min_heap_alfabetico(heap);
    }

    // Destruir el heap y retornar
    destruir_heap_alfabetico(heap);
    return ordenados;
}

/*
 * Ordena un array de artículos por nombre de archivo (ruta) alfabéticamente
 * E: articulos - array de artículos
 *    n - cantidad de artículos
 * S: nuevo array con artículos ordenados por ruta
 */
struct articulo* ordenar_por_nombre_archivo(struct articulo* articulos, int n) {
    // Validar 
    if (articulos == NULL || n <= 0) {
        return NULL;
    }

    // Crear heap alfabético
    struct heap_alfabetico* heap = crear_heap_alfabetico(n);
    if (heap == NULL) {
        fprintf(stderr, "Error: no se pudo crear heap para ordenar por archivo.\n");
        return NULL;
    }

    // Insertar todos los artículos usando la RUTA como llave
    for (int i = 0; i < n; i++) {
        insertar_heap_alfabetico(heap, articulos[i], articulos[i].ruta);
    }

    // Crear array para artículos ordenados
    struct articulo* ordenados = (struct articulo*) calloc(n, sizeof(struct articulo));
    if (ordenados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para array ordenado.\n");
        destruir_heap_alfabetico(heap);
        return NULL;
    }

    // se extraen artículos del heap (saldrán ordenados por su ruta)
    for (int i = 0; i < n; i++) {
        ordenados[i] = extraer_min_heap_alfabetico(heap);
    }

    // Destruir el heap y retornar
    destruir_heap_alfabetico(heap);
    return ordenados;
}