#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Lee una línea del archivo y la parsea en un artículo
 * Formato: nombre|apellido|titulo|ruta|año|resumen|
 * E: linea, string con el formato especificado
 * S: estructura articulo con los datos parseados
 */
static struct articulo parsear_linea(char* linea) {
    // Remover salto de línea si existe
    size_t len = strlen(linea);
    if (len > 0 && linea[len-1] == '\n') {
        linea[len-1] = '\0';
    }
    if (len > 1 && linea[len-2] == '\r') {
        linea[len-2] = '\0';
    }
    
    // Crear artículo usando la función de heap.c
    char* nombre = NULL;
    char* apellido = NULL;
    char* titulo = NULL;
    char* ruta = NULL;
    int ano = 0;
    char* resumen = NULL;
    
    // Parsear usando strtok con delimitador "|"
    char* token;
    int campo = 0;
    
    token = strtok(linea, "|");
    while (token != NULL && campo < 6) {
        switch(campo) {
            case 0: // Nombre autor
                nombre = token;
                break;
            case 1: // Apellido autor
                apellido = token;
                break;
            case 2: // Título
                titulo = token;
                break;
            case 3: // Ruta
                ruta = token;
                break;
            case 4: // Año
                ano = atoi(token);
                break;
            case 5: // Resumen
                resumen = token;
                break;
        }
        campo++;
        token = strtok(NULL, "|");
    }
    
    // Crear y retornar el artículo
    return crear_articulo(nombre, apellido, titulo, ruta, ano, resumen);
}

/*
 * Carga todos los artículos desde el archivo índice
 * E: nombre_archivo que es la ruta al archivo.txt
 *    total q es el puntero donde guardar la cantidad de artículos cargados
 * S: array dinámico con todos los artículos, o null si falla
 */
struct articulo* cargar_articulos(const char* nombre_archivo, int* total) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", nombre_archivo);
        return NULL;
    }
    
    // Contar líneas primero
    int num_articulos = 0;
    char buffer[4096]; // Buffer grande para líneas largas (resúmenes extensos)
    
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        // Ignorar líneas vacías
        if (strlen(buffer) > 1) {
            num_articulos++;
        }
    }
    
    printf("Se encontraron %d articulos en el archivo.\n", num_articulos);
    
    // Volver al inicio del archivo
    rewind(archivo);
    
    // Asignar memoria para todos los artículos
    struct articulo* articulos = (struct articulo*) malloc(num_articulos * sizeof(struct articulo));
    if (articulos == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para %d articulos\n", num_articulos);
        fclose(archivo);
        return NULL;
    }
    
    // Leer y parsear cada línea
    int i = 0;
    while (fgets(buffer, sizeof(buffer), archivo) != NULL && i < num_articulos) {
        // Ignorar líneas vacías
        if (strlen(buffer) > 1) {
            articulos[i] = parsear_linea(buffer);
            i++;
        }
    }
    
    fclose(archivo);
    *total = num_articulos;
    
    printf("Se cargaron %d articulos exitosamente.\n", num_articulos);
    return articulos;
}