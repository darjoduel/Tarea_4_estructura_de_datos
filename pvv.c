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

        } else if (cmd_hash == start || strcmp(command, "start") == 0) {//START
            if (cities != NULL) {
                printf("Ya existe un grafo. Usa 'exit' primero.\n");
                continue;
            }

            cities = (Graph*)malloc(sizeof(Graph));
            //Detecta e tamaño real de las ciudades en map.txt
            cities->numVertex = CharToNum(arg);
            if (cities->numVertex < 4) {
                cities->numVertex = 4;
                printf("Usando tamano minimo de 4 ciudades.\n");
                continue;
            }
            
            printf("numero de ciudades en el grafo antes de createGraph: %d\n", cities->numVertex);
            continue;
            
            /*char unique_letters[26] = {0};  // Asumimos max 26 letras
            int letter_count = 0;
            FILE *source_graph = fopen("map.txt", "r");
            if (!source_graph) {
                printf("Error: No existe 'map.txt'. Crea el archivo primero.\n");
                continue;
            }
            char v1, v2;
            int cost;
            while (fscanf(source_graph, " %c %c %d", &v1, &v2, &cost) == 3) {
                int found1 = 0, found2 = 0;
                for (int i = 0; i < letter_count; i++) {
                    if (unique_letters[i] == v1) found1 = 1;
                    if (unique_letters[i] == v2) found2 = 1;
                }
                if (!found1) unique_letters[letter_count++] = v1;
                if (!found2) unique_letters[letter_count++] = v2;
            }
            fclose(source_graph);
            real_size = letter_count;*/

            /*// Ignoramos 'n' del usuario si lo dio; usamos real_size
            if (pvv_create(real_size) != 0) {
                printf("Error al crear archivo 'graphdata'.\n");
                continue;
            }*/
            /*cities = createGraph(real_size, "map.txt");
            if (cities) {
                printf("Grafo creado con %d ciudades (detectadas automáticamente de 'map.txt').\n", real_size);
                ShowGraph(cities);
            } else {
                printf("Error al crear el grafo.\n");
            }*/

        } else if (cmd_hash == read || strcmp(command, "read") == 0) {//READ
            if (cities == NULL) {
                printf("ERROR: Primero usa 'start'\n");
                continue;
            }
            cities = createGraph(cities->numVertex, arg);
            if (cities) {
                printf("Grafo creado con %d ciudades (detectadas automaticamente de 'map.txt').\n", cities->numVertex);
                ShowGraph(cities);
            } else {
                printf("Error al crear el grafo.\n");
            }
            if (pvv_read(arg) == 0) {
                printf("Enlaces cargados correctamente desde '%s'\n", arg);
                printf("Buscando ciclo hamiltoniano...\n");
                findHamiltonianCycle(cities);
            } else {
                printf("Error: No se pudo leer el archivo '%s'\n", arg);
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