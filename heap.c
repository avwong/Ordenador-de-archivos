#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*crea una copia en memoria de una cadena de caracteres (string duplicate)
E: cadena de caracteres original
S: puntero a la nueva cadena de caracteres en memoria
R: que la cadena no sea NULL
*/
static char* mi_strdup(const char* cadena) {
    if (cadena == NULL) return NULL;
    size_t len = strlen(cadena) + 1; // +1 para el carácter nulo
    char* copia = calloc(1, len); //asinga memoria en el heap para la copia
    if (copia != NULL) {
        memcpy(copia, cadena, len); //copia el contenido de la cadena original a la nueva ubicación
    }
    return copia;
}

/*crea un artículo con la información proporcionada
E: nombre, apellido, titulo, ruta, ano, resumen del artículo
S: estructura articulo con los datos copiados en memoria
R: que los parámetros no sean NULL (excepto ano que es int)
*/
struct articulo crear_articulo(const char* nombre, const char* apellido, const char* titulo, const char* ruta, int ano, const char* resumen) {
    struct articulo art;
    art.nombre_autor = mi_strdup(nombre);
    art.apellido_autor = mi_strdup(apellido);
    art.titulo_articulo = mi_strdup(titulo);
    art.ruta = mi_strdup(ruta);
    art.ano = ano;
    art.resumen = mi_strdup(resumen);
    return art;
}

/*libera la memoria ocupada por un artículo
E: puntero al artículo a liberar
S: void
R: que el puntero del articulo no sea NULL
*/
void liberar_articulo(struct articulo* art) {
    //validacion
    if (art == NULL) return;

    free(art->nombre_autor);
    free(art->apellido_autor);
    free(art->titulo_articulo);
    free(art->ruta);
    free(art->resumen);
    art->nombre_autor = NULL;
    art->apellido_autor = NULL;         
    art->titulo_articulo = NULL;
    art->ruta = NULL;
    art->resumen = NULL;
}

/*cuenta la cantidad de palabras en una cadena de caracteres
E: texto (cadena de caracteres)
S: cantidad de palabras en la cadena
R: que el texto no sea NULL
*/
int contar_palabras(const char *texto) {
    //validaciones
    if (texto == NULL || *texto == '\0') {
        return 0;
    }

    int contador = 0;
    int en_palabra = 0; //para saber si esta o no dentro de una palabra (1 = si, 0 = no)

    while (*texto) {
        if (*texto == ' ' || *texto == '\n' || *texto == '\t') {
            en_palabra = 0;
        } else {
            if (!en_palabra) {
                contador++;
                en_palabra = 1;
            }
        }
        texto++;
    }

    return contador;
}