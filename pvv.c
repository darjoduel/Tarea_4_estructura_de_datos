#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "functions.h"
#include "structures.h"

Graph* cities = NULL;  // Grafo global

// Hashes simples
#define help       61964
#define start      743628
#define read       71842
#define all        5560
#define exit_cmd   60838
#define hamiltonian 37604

// Convierte string a un "hash" simple 
unsigned int simple_hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = hash * 31 + (*str++);
    }
    return hash;
}

int CharToNum(char *str){// convierte cadena a numero entero positivo
    int num = atoi(str);
    if(num < 0){
        num*= -1;
    }
    return num;
}

int main() {
    char line[256];
    char command[50];
    char arg[200];
    //cities->numVertex = 0;

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

        unsigned int cmd_hash = simple_hash(command);

        if (cmd_hash == help || strcmp(command, "help") == 0) {//HELP
            printf("Comandos disponibles:\n");
            printf("  start [n]        → Crear grafo (n opcional; se detecta auto del archivo)\n");
            printf("  read <archivo>   → Leer conexiones desde archivo\n");
            printf("  all              → Mostrar grafo actual\n");
            printf("  hamil            → Buscar ciclo hamiltoniano\n");
            printf("  exit             → Salir y liberar memoria\n");
            printf("  help             → Mostrar esta ayuda\n\n");

        } else if (cmd_hash == start || strcmp(command, "start") == 0) {
            if (cities != NULL) {
                printf("Ya existe un grafo. Usa 'exit' primero.\n");
                continue;
            }

            int n = CharToNum(arg);
            if (n < 4) {
                n = 4;
                printf("Usando tamaño mínimo de 4 ciudades.\n");
            }
    
            cities = (Graph*)malloc(sizeof(Graph));
            if (!cities) {
                printf("Error al asignar memoria para el grafo.\n");
            continue;
            }
    
            cities->numVertex = n;
            cities->vertList = NULL; // Se inicializará en createGraph
    
            printf("Grafo preparado para %d ciudades.\n", n);
            printf("Usa 'read <archivo>' para cargar las conexiones.\n");

        } else if (cmd_hash == read || strcmp(command, "read") == 0) {//READ
             if (cities == NULL) {
                printf("ERROR: Primero usa 'start'\n");
                continue;
            }

            /*guarda el numero de vertices antes de intentar cargar*/
            int numVert = cities->numVertex;

            //intenta crear el grafo desde el archivo SIN liberar cities aún
            Graph* newGraph = createGraph(numVert, arg);
    
            if (newGraph){/*si se creo bien, ahora si libera cities*/
                          /*liberar todas las aristas del grafo anterior*/
                if(cities->vertList){
                    for (int i = 0; i < cities->numVertex; i++) {
                        Edge *curr = cities->vertList[i].edgeListHead;
                        while (curr) {
                            Edge *temp = curr;
                            curr = curr->nextEdge;
                            free(temp);
                        }
                    }
                    free(cities->vertList);
                }
                free(cities);
                /*ahora se asigna el nuevo grafo*/
                cities = newGraph;
                printf("Grafo creado con %d ciudades.\n", cities->numVertex);
                ShowGraph(cities);

                // Actualizar graphdata con el archivo fuente
                    if (pvv_read(arg) == 0) {
                        printf("\nBuscando ciclo hamiltoniano...\n");
                        findHamiltonianCycle(cities);
                    } else {
                        printf("Advertencia: No se pudo actualizar graphdata.\n");
                    }
            } else {
                printf("Error al crear el grafo desde '%s'.\n", arg);
            }

        } else if (cmd_hash == all || strcmp(command, "all") == 0) {//ALL
            if (cities) {
                ShowGraph(cities);
            } else {
                printf("No hay grafo creado.\n");
            }

        } else if (cmd_hash == hamiltonian || strcmp(command, "hamil") == 0) {
            if (cities) {
                findHamiltonianCycle(cities);
            } else {
                printf("ERROR: Primero crea el grafo con 'start' y carga datos con 'read'\n");
            }

        } else if (cmd_hash == exit_cmd || strcmp(command, "exit") == 0) {//EXIT
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