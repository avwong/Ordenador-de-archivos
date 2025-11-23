#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "heap.h" // para struct articulo y los heaps

// limpia el buffer de entrada cuando hay basura
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/*muestra el menú principal, valida la opción y pregunta cuántos resultados mostrar
E: totalArticulos (cuántos artículos hay en memoria), cantidadMostrar (puntero donde guardar cuántos artículos mostrar)
S: opción elegida (1-5)
R: que totalArticulos sea mayor a 0
*/
int mostrar_menu_principal(int totalArticulos, int *cantidadMostrar) {
    int opcion = 0;

    do {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1. Ordenar por titulo (A-Z)\n");
        printf("2. Ordenar por cantidad de palabras en el titulo\n");
        printf("3. Ordenar por nombre de archivo (ruta)\n");
        printf("4. Ordenar por año\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion (1-5): ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada invalida. Intente de nuevo.\n");
            limpiar_buffer();
            opcion = 0; // fuerza repetir
            continue;
        }

        limpiar_buffer(); // limpia el \n que queda

        if (opcion < 1 || opcion > 5) {
            printf("Opcion fuera de rango. Intente de nuevo.\n");
        }

    } while (opcion < 1 || opcion > 5);

    // Si elige una opcion de ordenamiento (1-4), preguntar cuántos resultados mostrar
    if (opcion >= 1 && opcion <= 4) {
        int cant = 0;
        do {
            printf("Hay %d articulos cargados.\n", totalArticulos);
            printf("¿Cuantos articulos desea mostrar? (1 - %d): ", totalArticulos);

            if (scanf("%d", &cant) != 1) {
                printf("Entrada invalida. Intente de nuevo.\n");
                limpiar_buffer();
                cant = 0;
                continue;
            }

            limpiar_buffer();

            if (cant < 1 || cant > totalArticulos) {
                printf("Cantidad fuera de rango. Intente de nuevo.\n");
            }
        } while (cant < 1 || cant > totalArticulos);

        *cantidadMostrar = cant;
    }

    return opcion;
}

/*imprime en pantalla los artículos ordenados de forma clara y legible para el usuario
E: articulos (arreglo de artículos), n (cantidad de artículos a mostrar), criterio (criterio de ordenamiento)
S: void
R: que articulos no sea NULL, que n sea mayor a 0
*/
void imprimir_articulos(struct articulo* articulos, int n, const char* criterio) {
    printf("\n");
    printf("========================================\n");
    printf("  RESULTADOS: Ordenados por %s\n", criterio);
    printf("========================================\n\n");

    for (int i = 0; i < n; i++) {
        printf("[Artículo %d]\n", i + 1);
        printf("  Título:   %s\n", articulos[i].titulo_articulo);
        printf("  Autor:    %s %s\n", articulos[i].nombre_autor, articulos[i].apellido_autor);
        printf("  Año:      %d\n", articulos[i].ano);
        printf("  Archivo:  %s\n", articulos[i].ruta);
        printf("  Resumen:  %s\n", articulos[i].resumen);
        printf("----------------------------------------\n\n");
    }
    printf("Total de artículos mostrados: %d\n", n);
    printf("----------------------------------------\n");
}

int main() {
    int totalArticulos = 0;
    
    printf("===========================================\n");
    printf("  SISTEMA DE ORDENAMIENTO DE ARTICULOS\n");
    printf("  Sobre corrupcion en America Latina\n");
    printf("===========================================\n\n");
    
    // acá se carga los artículos del archivo.txt
    printf("Cargando articulos desde archivo.txt...\n");
    struct articulo* articulos = cargar_articulos("archivo.txt", &totalArticulos);
    
    if (articulos == NULL) {
        fprintf(stderr, "Error: No se pudieron cargar los articulos.\n");
        fprintf(stderr, "Verifique que el archivo 'archivo.txt' existaaa\n");
        return 1;
    }
    
    printf("\n Articulos cargados exitosamente :) \n");
    printf("Total de articulos disponibles: %d\n", totalArticulos);
    
    int continuar = 1;
    
    while (continuar) {
        int cantidadMostrar = 0;
        int opcion = mostrar_menu_principal(totalArticulos, &cantidadMostrar);
        
        struct articulo* ordenados = NULL;
        const char* criterio = NULL;
        
        switch(opcion) {
            case 1: // Ordenar por título
                printf("\nOrdenando por titulo...\n");
                ordenados = ordenar_por_titulo(articulos, totalArticulos);
                criterio = "titulo (A-Z)";
                break;
                
            case 2: // Ordenar por cantidad de palabras
                printf("\nOrdenando por cantidad de palabras en el titulo...\n");
                ordenados = ordenar_por_palabras_titulo(articulos, totalArticulos);
                criterio = "cantidad de palabras en el titulo";
                break;
                
            case 3: // Ordenar por nombre de archivo
                printf("\nOrdenando por nombre de archivo...\n");
                ordenados = ordenar_por_nombre_archivo(articulos, totalArticulos);
                criterio = "nombre de archivo";
                break;
                
            case 4: // Ordenar por año
                printf("\nOrdenando por anio...\n");
                ordenados = ordenar_por_ano(articulos, totalArticulos);
                criterio = "año";
                break;
                
            case 5: // Salir
                printf("\n===========================================\n");
                printf("Gracias por usar nuestro sistema, byebye :)\n");
                printf("===========================================\n");
                continuar = 0;
                break;
        }
        
        // Mostrar resultados si se ordenó correctamente
        if (ordenados != NULL) {
            imprimir_articulos(ordenados, cantidadMostrar, criterio);
            
            // Liberar memoria del array ordenado
            free(ordenados);
        }
    }
    
    // Liberar memoria de los artículos originales
    printf("\nLiberando memoria...\n");
    for (int i = 0; i < totalArticulos; i++) { // liberar cada artículo
        liberar_articulo(&articulos[i]); // función de heap.c
    }
    free(articulos);
    
    return 0;
}
