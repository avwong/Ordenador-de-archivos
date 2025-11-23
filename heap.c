#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Función encargada de crear una copia en memoria de una cadena de caracteres
"String duplicate"
E: cadena de caracteres original
S: puntero a la nueva cadena de caracteres en memoria
*/
static char* mi_strdup(const char* cadena) {
    if (cadena == NULL) return NULL;
    size_t len = strlen(cadena) + 1; // +1 para el carácter nulo
    char* copia = calloc(1, len); // Asinga memoria en el heap para la copia
    if (copia != NULL) {
        memcpy(copia, cadena, len); // Copia el contenido de la cadena original a la nueva ubicación
    }
    return copia;
}

/*Función que cuenta la cantidad de palabras en una cadena de caracteres
E: cadena de caracteres original
S: cantidad de palabras en la cadena 
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

/*Función que libera la memoria ocupada por un artículo
E: puntero al artículo a liberar        
S: -
*/ 
void liberar_articulo(struct articulo* art) {
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
/*Función que cuenta la cantidad de palabras en una cadena de caracteres
E: cadena de caracteres original        
S: cantidad de palabras en la cadena 
*/  
int contar_palabras(const char *texto) {
    if (texto == NULL || *texto == '\0') {
        return 0;
    }

    int contador = 0;
    int en_palabra = 0;

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