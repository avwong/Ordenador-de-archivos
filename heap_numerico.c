#include "heap.h"
#include <stdio.h>   // fprintf
#include <stdlib.h>  // malloc, free, realloc
#include <string.h>  // strcmp, strlen, memcpy

/*
E: puntero al heap , indices de los nodos a intercambiar
S: void
R: el heap debe existir, los indices deben ser validos
*/
static void intercambiar_nodos_numerico(struct heap_numerico* heap, int i, int j) {
    
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
static void asegurar_capacidad_numerico(struct heap_numerico* heap) {
    
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
static void subir_numerico(struct heap_numerico* heap, int hijo) {
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

/*
E: el heap  y el indice del nodo padre a mover hacia abajo
S: void
R: que el heap exista, asegurarse que el indice sea valido
*/
static void bajar_numerico(struct heap_numerico* heap, int padre) {
    int izquierda, derecha, menor;

    while (1) { //para que se haga infinitamente (hasta que haya un break)
        
        //formulas para llegar a hijos
        izquierda = 2 * padre + 1;
        derecha = 2 * padre + 2;
        menor = padre;

        //verificar que de ese lado existan hijos y comparar llaves
        if (izquierda < heap->tamano && heap->nodos[izquierda].llave < heap->nodos[menor].llave) {
            menor = izquierda;
        }
        if (derecha < heap->tamano && heap->nodos[derecha].llave < heap->nodos[menor].llave) {
            menor = derecha;
        }

        //para que no se intente de intercambiar con si mismo en caso de que el padre sea menor
        if (menor != padre) { 
            intercambiar_nodos_numerico(heap, padre, menor);
            padre = menor;
        } else { 
            break;
        }
    }
}

/*
E: capacidad inicial del heap
S: puntero al heap que se acaba de crear, NULL si falla
R: que el heap se cree correctamente
*/
struct heap_numerico* crear_heap_numerico(int capacidad_inicial) {
    struct heap_numerico* heap = calloc(1, sizeof(struct heap_numerico));
    
    //por si no se le asigno memoria correcatmente
    if (heap == NULL) {
        printf("Error: no se pudo crear el heap numerico.\n");
        return NULL;
    }

    //memoria para el arreglo de nodos
    heap->nodos = calloc(capacidad_inicial, sizeof(struct nodo_heap_numerico));
    if (heap->nodos == NULL) {
        printf("Error: no se pudo asignar memoria para los nodos del heap numerico.\n");
        free(heap); //libera el espacio que se agarro para el heap
        return NULL;
    }

    heap->tamano = 0; //inicia en 0 xq aun no tiene contenido
    heap->capacidad = capacidad_inicial;
    return heap;
}

/*
verifica si el heap esta vacio
E: puntero al heap
S: 1 si esta vacio, 0 si no
R: que el heap exista
*/
int heap_numerico_vacio(struct heap_numerico* heap) {
    if (heap == NULL || heap->tamano == 0) {
        return 1; //etsa vacio o no existe
    }
    return 0; //no esta vacio
}

/*
inserta un articulo en el heap
E: puntero al heap, articulo a insertar, llave
S: void
R: que el heap exista
*/
void insertar_heap_numerico(struct heap_numerico* heap, struct articulo articulo, int llave) {
    if (heap == NULL) {
        printf("Error: el heap no existe.\n");
        return;
    }

    //asegurar que haya espacio para un nodo mas
    asegurar_capacidad_numerico(heap);

    //insertar el nuevo nodo al final
    heap->nodos[heap->tamano].articulo = articulo;
    heap->nodos[heap->tamano].llave = llave;

    heap->tamano++; //el heap incrementa con un nodo mas

    //ver si subir el nodo para mantener la propiedad del heap
    subir_numerico(heap, heap->tamano - 1);
}

/*
Extrae el articulo con la llave minima del heap
E: puntero al heap
S: articulo con la llave minima
R: que el heap exista y no este vacio
*/
struct articulo extraer_min_heap_numerico(struct heap_numerico* heap) {
    
    //para retornar en caso de error
    struct articulo art_vacio = {NULL, NULL, NULL, NULL, 0};
    
    //validaciones
    if (heap == NULL || heap->tamano == 0) {
        printf("Error: no se puede extraer de un heap vacio o inexistente.\n");
        return art_vacio;
    }

    struct nodo_heap_numerico min_nodo = heap->nodos[0]; //el nodo con la llave minima esta en la raiz

    heap->tamano--; //disminuye el tamaño del heap

    //si aun hay nodos, mover el ultimo a la raiz y bajar para mantener la propiedad del heap
    if (heap->tamano > 0) {
        heap->nodos[0] = heap->nodos[heap->tamano];
        bajar_numerico(heap, 0);
    }

    return min_nodo.articulo;
}

/*
E: puntero al heap a destruir
S: void
R: que el heap exista
*/
void destruir_heap_numerico(struct heap_numerico* heap) {
    
    //validacion
    if (heap == NULL) {
        printf("Error: el heap no existe.\n");
        return;
    }

    //liberrar el arreglo de nodos y el heap entero
    free(heap->nodos);
    free(heap);
}

//Funciones de ordenamiento numérico

/*
 Ordena un array de artículos por año (menor a mayor)
 E: articulos (arreglo de artículos), n = cantidad de artículos
 S: nuevo arreglo con artículos ordenados por año
 R: que el arreglo exista y hayan artículos
 */
struct articulo* ordenar_por_ano(struct articulo* articulos, int n) {
    //validar
    if (articulos == NULL || n <= 0) {
        return NULL;
    }

    //crear heap numérico
    struct heap_numerico* heap = crear_heap_numerico(n);
    if (heap == NULL) {
        printf("Error: no se pudo crear heap para ordenar por año.\n");
        return NULL;
    }

    //insertar todos los artículos usando el año como llave
    for (int i = 0; i < n; i++) {
        insertar_heap_numerico(heap, articulos[i], articulos[i].ano);
    }

    //crear arreglo para artículos ordenados
    struct articulo* ordenados = calloc(n, sizeof(struct articulo));
    if (ordenados == NULL) {
        printf("Error: no se pudo asignar memoria para arreglo ordenado.\n");
        destruir_heap_numerico(heap);
        return NULL;
    }

    //extraer artículos del heap que salen ya ordenados por año
    for (int i = 0; i < n; i++) {
        ordenados[i] = extraer_min_heap_numerico(heap);
    }

    // destruir el heap y retornar los artículos ordenados
    destruir_heap_numerico(heap);
    return ordenados;
}

/*
Ordena un array de artículos por cantidad de palabras en el título (menor a mayor)
E: articulos (arreglo de artículos), n = cantidad de artículos
S: nuevo arreglo con artículos ordenados por cantidad de palabras
R: que el arreglo exista y hayan artículos
 */
struct articulo* ordenar_por_palabras_titulo(struct articulo* articulos, int n) {
    //validar
    if (articulos == NULL || n <= 0) {
        return NULL;
    }

    // Crear heap numérico
    struct heap_numerico* heap = crear_heap_numerico(n);
    if (heap == NULL) {
        printf("Error: no se pudo crear heap para ordenar por palabras.\n");
        return NULL;
    }

    //insertar todos los artículos usando la cantidad de palabras como llave
    for (int i = 0; i < n; i++) {
        int num_palabras = contar_palabras(articulos[i].titulo_articulo);
        insertar_heap_numerico(heap, articulos[i], num_palabras);
    }

    //crear arreglo para artículos ordenados
    struct articulo* ordenados = calloc(n, sizeof(struct articulo));
    if (ordenados == NULL) {
        printf("Error: no se pudo asignar memoria para arreglo ordenado.\n");
        destruir_heap_numerico(heap);
        return NULL;
    }

    //extraer artículos del heap que salen ya ordenados por cantidad de palabras
    for (int i = 0; i < n; i++) {
        ordenados[i] = extraer_min_heap_numerico(heap);
    }

    //destruir el heap y retornar los artículos ordenados
    destruir_heap_numerico(heap);
    return ordenados;
}