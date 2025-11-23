#include "heap.h"
#include <stdio.h>   // fprintf
#include <stdlib.h>  // malloc, free, realloc
#include <string.h>  // strcmp, strlen, memcpy



/*copia una llave alfabética en memoria
E: llave a copiar
S: puntero a la copia de la llave
R: que la llave no sea NULL
*/
static char* copiar_llave_alfabetica(const char* llave) {
    if (llave == NULL) return NULL;
    size_t len = strlen(llave) + 1; // +1 para el carácter nulo
    char* copia = calloc(1, len); // Asigna memoria en el heap para la copia
    if (copia != NULL) {
        memcpy(copia, llave, len); // Copia el contenido de la cadena original a la nueva ubicación
    }
    return copia;
}

//funciones del heap alfabético

/*intercambia dos nodos del heap alfabético
E: punteros a los nodos a intercambiar
S: void
R: que los punteros no sean NULL
*/
static void intercambiar_nodos_alfabetico(struct nodo_heap_alfabetico* a    , struct nodo_heap_alfabetico* b) {
    struct nodo_heap_alfabetico temp = *a;
    *a = *b;
    *b = temp;
}

/*asegura que siempre haya espacio en el heap para agregar un nodo mas en caso de que se llegue a llenar el heap
E: puntero al heap alfabético
S: void
R: que exista el heap, que tenga capacidad mayor a 0
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

/*intercambia un nodo hijo con su padre si el nodo hijo es menor alfabéticamente que el padre
E: heap y el indice del nodo a mover hacia arriba
S: void
R: que el heap exista, asegurarse que el indice sea valido
*/
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

/*mueve un nodo hacia abajo en el heap para mantener la propiedad del min heap
E: el heap y el indice del nodo padre a mover hacia abajo
S: void
R: que el heap exista, asegurarse que el indice sea valido
*/
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

/*crea un heap alfabético
E: capacidad inicial del heap
S: puntero al heap que se acaba de crear, NULL si falla
R: que el heap se cree correctamente
*/
struct heap_alfabetico* crear_heap_alfabetico(int capacidad_inicial) {
    struct heap_alfabetico* heap = calloc(1, sizeof(struct heap_alfabetico));
    if (heap == NULL) {
        fprintf(stderr, "Error: no se pudo crear el heap alfabetico.\n");
        return NULL;
    }

    heap->nodos = calloc(capacidad_inicial, sizeof(struct nodo_heap_alfabetico));
    if (heap->nodos == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para los nodos del heap alfabetico.\n");
        free(heap);
        return NULL;
    }

    heap->tamano = 0;
    heap->capacidad = capacidad_inicial;
    return heap;
}

/*verifica si el heap esta vacio
E: puntero al heap
S: 1 si esta vacio, 0 si no
R: que el heap exista
*/
int heap_alfabetico_vacio(struct heap_alfabetico* heap) {
    return (heap == NULL || heap->tamano == 0);
}

/*inserta un articulo en el heap alfabético
E: puntero al heap, articulo a insertar, llave alfabética
S: void
R: que el heap exista, que la llave no sea NULL
*/
void insertar_heap_alfabetico(struct heap_alfabetico *heap, struct articulo articulo, const char *llave) {
    //validaciones
    if (heap == NULL || llave == NULL) return;

    asegurar_capacidad_alfabetico(heap);

    int idx = heap->tamano;

    //copiar el articulo por valor
    heap->nodos[idx].articulo = articulo;

    //guardar copia de la llave
    heap->nodos[idx].llave = copiar_llave_alfabetica(llave);

    heap->tamano++;

    subir_alfabetico(heap, idx);
}

/*extrae el articulo con la llave alfabética mínima del heap
E: puntero al heap alfabético
S: articulo con la llave alfabética mínima
R: que el heap exista y no este vacio
*/
struct articulo extraer_min_heap_alfabetico(struct heap_alfabetico* heap) {
    //artículo vacío por si algo sale mal
    struct articulo vacio = {NULL, NULL, NULL, NULL, 0, NULL};

    //validaciones
    if (heap == NULL || heap->tamano == 0) {
        return vacio;
    }

    // guardar el nodo mínimo para devolverlo
    struct nodo_heap_alfabetico min_nodo = heap->nodos[0];

    //se reemplaza la raíz con el último nodo y se reduce el tamaño
    heap->tamano--;
    if (heap->tamano > 0) {
        heap->nodos[0] = heap->nodos[heap->tamano];
        bajar_alfabetico(heap, 0);
    }

    //se libera la memoria de la llave del nodo mínimo
    free(min_nodo.llave);

    return min_nodo.articulo;
}

/*destruye el heap alfabético y libera la memoria
E: puntero al heap a destruir
S: void
R: que el heap exista
*/
void destruir_heap_alfabetico(struct heap_alfabetico* heap) {
    //validacion
    if (heap == NULL) return;

    //liberar la memoria de las llaves y el arreglo de nodos
    for (int i = 0; i < heap->tamano; i++) {
        free(heap->nodos[i].llave);
    }

    free(heap->nodos);
    free(heap);
}

//Funciones de ordenamiento alfabético

/*ordena un arreglo de artículos por título alfabéticamente (A-Z)
E: articulos (arreglo de artículos), n = cantidad de artículos
S: nuevo arreglo con artículos ordenados por título
R: que el arreglo exista y hayan artículos
*/
struct articulo* ordenar_por_titulo(struct articulo* articulos, int n) {
    //Validar
    if (articulos == NULL || n <= 0) {
        return NULL;
    }

    //crear heap alfabético
    struct heap_alfabetico* heap = crear_heap_alfabetico(n); // capacidad inicial = n
    if (heap == NULL) {
        fprintf(stderr, "Error: no se pudo crear heap para ordenar por titulo.\n");
        return NULL;
    }

    //insertar todos los artículos usando el título como llave
    for (int i = 0; i < n; i++) {
        insertar_heap_alfabetico(heap, articulos[i], articulos[i].titulo_articulo);
    }

    //crear arreglo para artículos ordenados
    struct articulo* ordenados = calloc(n, sizeof(struct articulo));
    if (ordenados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para array ordenado.\n");
        destruir_heap_alfabetico(heap);
        return NULL;
    }

    //extraer artículos del heap que salen ya ordenados alfabéticamente
    for (int i = 0; i < n; i++) {
        ordenados[i] = extraer_min_heap_alfabetico(heap);
    }

    //destruir el heap y retornar
    destruir_heap_alfabetico(heap);
    return ordenados;
}

/*ordena un arreglo de artículos por nombre de archivo (ruta) alfabéticamente
E: articulos (arreglo de artículos), n = cantidad de artículos
S: nuevo arreglo con artículos ordenados por ruta
R: que el arreglo exista y hayan artículos
*/
struct articulo* ordenar_por_nombre_archivo(struct articulo* articulos, int n) {
    //validar 
    if (articulos == NULL || n <= 0) {
        return NULL;
    }

    //crear heap alfabético
    struct heap_alfabetico* heap = crear_heap_alfabetico(n);
    if (heap == NULL) {
        fprintf(stderr, "Error: no se pudo crear heap para ordenar por archivo.\n");
        return NULL;
    }

    //insertar todos los artículos usando la RUTA como llave
    for (int i = 0; i < n; i++) {
        insertar_heap_alfabetico(heap, articulos[i], articulos[i].ruta);
    }

    //crear arreglo para artículos ordenados
    struct articulo* ordenados = calloc(n, sizeof(struct articulo));
    if (ordenados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para array ordenado.\n");
        destruir_heap_alfabetico(heap);
        return NULL;
    }

    //extraer artículos del heap (saldrán ordenados por su ruta)
    for (int i = 0; i < n; i++) {
        ordenados[i] = extraer_min_heap_alfabetico(heap);
    }

    //destruir el heap y retornar
    destruir_heap_alfabetico(heap);
    return ordenados;
}