#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "functions.h"
#include "structures.h"

Graph* cities = NULL;  // Grafo global

int main(){

    char line[256];
    char command[50];
    char arg[200];

    printf("=== Bienvenido al Problema del Viajante (PVV) ===\n");
    printf("Escribe 'help' para ver los comandos disponibles.\n\n");

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            break; // Ctrl+D o error
        }

        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0){
            continue;
        }
        // Separar comando y argumento
        if (sscanf(line, "%49s %199[^\n]", command, arg) < 1) {
            continue;
        }

        if (strcmp(command, "help") == 0) {//HELP
            printf("Comandos disponibles:\n");
            printf("  start [n]        → Crear grafo (n opcional; se detecta auto del archivo)\n");
            printf("  read <archivo>   → Leer conexiones desde archivo\n");
            printf("  all              → Mostrar grafo actual\n");
            printf("  hamil            → Buscar ciclo hamiltoniano\n");
            printf("  exit             → Salir y liberar memoria\n");
            printf("  help             → Mostrar esta ayuda\n\n");

        } else if (strcmp(command, "start") == 0) {
            cities = pvv_create(cities, arg);
            if(cities){
                printf("Grafo inicializado con %d ciudades.\n", cities->numVertex);
                arg[0] = '\0';
            } else {
                printf("ERROR: No se pudo crear el grafo.\n");
            }
            continue;
        } else if (strcmp(command, "read") == 0) {//READ
            cities = pvv_read(cities, arg);
            if (cities->vertList) {
                printf("Grafo leido desde '%s' con exito.\n", arg);
                pvv_all(cities);
                findHamiltonianCycle(cities);
            } else {
                printf("ERROR: No se pudo leer el grafo desde '%s'.\n", arg);
            }

        } else if (strcmp(command, "all") == 0) {//ALL
            if (cities) {
                pvv_all(cities);
            } else {
                printf("No hay grafo creado.\n");
            }
        } else if (strcmp(command, "hamil") == 0) {//HAMIL
            if (cities && cities->vertList) {
                findHamiltonianCycle(cities);
            } else {
                printf("ERROR: Primero crea el grafo con 'start' y carga datos con 'read'\n");
            }

        } else if (strcmp(command, "exit") == 0) {//EXIT
            if (cities) {
                // Liberación
                for (int i = 0; i < cities->numVertex; i++) {
                    Edge *curr = cities->vertList[i].edgeListHead;
                    while (curr) {
                        Edge *temp = curr;
                        curr = curr->nextEdge;
                        free(temp);
                    }
                }
                free(cities->vertList);
                free(cities);
                cities = NULL;
                printf("Memoria liberada completamente. ¡Adios!\n");
            }
            break;

        } else {
            printf("Comando no reconocido. Escribe 'help' para ver la lista.\n");
        }
        printf("\n");
    }

    return 0;
}