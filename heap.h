#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <string.h>

//estructura de un articulo
struct articulo {
    char* nombre_autor;
    char* apellido_autor;
    char* titulo_articulo;
    char* ruta;
    int ano;
    char* resumen;
};

//HEAP NUMERICO: para cantidad de palabras en el titulo
struct nodo_heap_numerico {
    struct articulo articulo;
    int llave;
};

struct heap_numerico {
    struct nodo_heap_numerico* nodos;
    int tamano;
    int capacidad;
};

//funciones
struct heap_numerico* crear_heap_numerico(int capacidad_inicial);
void insertar_heap_numerico(struct heap_numerico* heap, struct articulo articulo, int llave);
struct articulo extraer_min_heap_numerico(struct heap_numerico* heap);
int heap_numerico_vacio(struct heap_numerico* heap);
void destruir_heap_numerico(struct heap_numerico* heap);

//HEAP ALFABETICO: para nombre de archivo o titulo
struct nodo_heap_alfabetico {
    struct articulo articulo;
    char* llave;  // Llave alfabética (puede ser título o filename)
};

struct heap_alfabetico {
    struct nodo_heap_alfabetico* nodos;
    int tamano;
    int capacidad;
};

//funciones
struct heap_alfabetico* crear_heap_alfabetico(int capacidad_inicial);
void insertar_heap_alfabetico(struct heap_alfabetico *heap, struct articulo articulo, const char *llave);
struct articulo extraer_min_heap_alfabetico(struct heap_alfabetico* heap);
int heap_alfabetico_vacio(struct heap_alfabetico* heap);
void destruir_heap_alfabetico(struct heap_alfabetico* heap);

// Funciones de ordenamiento alfabético
struct articulo* ordenar_por_titulo(struct articulo* articulos, int n);
struct articulo* ordenar_por_nombre_archivo(struct articulo* articulos, int n);

//FUNCIONES PARA LOS DOS
int contar_palabras(const char *texto);
struct articulo crear_articulo(const char* nombre, const char* apellido, const char* titulo, const char* ruta, int ano, const char* resumen);
void liberar_articulo(struct articulo* art);

// Función para cargar artículos desde archivo esto está en el file_parser.c
struct articulo* cargar_articulos(const char* nombre_archivo, int* total);

#endif