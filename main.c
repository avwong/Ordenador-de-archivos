#include <stdio.h>
#include "heap.h"   // para struct articulo y los heaps

// Limpia el buffer de entrada cuando hay basura
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/*
 * Muestra el menú principal, valida la opción
 * y también pregunta cuántos resultados mostrar.
 *
 * E: totalArticulos, cuántos artículos hay en memoria
 * S: return opción elegida1-5
 *    *cantidadMostrar-> cuántos artículos mostrar (si la opción es 1-4)
 */
int mostrar_menu_principal(int totalArticulos, int *cantidadMostrar) {
    int opcion = 0;

    do {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1. Ordenar por titulo (A-Z)\n");
        printf("2. Ordenar por cantidad de palabras en el titulo\n");
        printf("3. Ordenar por nombre de archivo (ruta)\n");
        printf("4. Ordenar por anio\n");
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

    // Si elige una opcion de ordenamiento (1-4),
    // preguntamos cuántos resultados mostrar
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