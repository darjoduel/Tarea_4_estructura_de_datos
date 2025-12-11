#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "structures.h"

Graph *pvv_create(Graph *map, char *size){
    if(map != NULL){
        printf("ERROR: El grafo ya ha sido creado.\n");
        return map;
    }
    int sizeNum = CharToNum(size);
    if (sizeNum < 4) {
        sizeNum = 4;
        printf("Usando tamano minimo de 4 ciudades.\n");
    }
    map = (Graph*)malloc(sizeof(Graph));
    if (!map) {
        printf("Error al asignar memoria para el grafo.\n");
    }
    map->numVertex = sizeNum;
    map->vertList = NULL; // Se inicializará en createGraph

    return map;
}

Graph *pvv_read(Graph *map, const char *filename){
    if (!map) {
        printf("ERROR: El grafo no ha sido inicializado. Usa 'start' primero.\n");
        return map;
    }
    Graph* newGraph = createGraph(map->numVertex, filename);
    if (!newGraph) {
        printf("ERROR: No se pudo crear el grafo desde '%s'.\n", filename);
        return map;
    }
    // Liberar el grafo anterior
    if (map->vertList) {
        for (int i = 0; i < map->numVertex; i++) {
            Edge *curr = map->vertList[i].edgeListHead;
            while (curr) {
                Edge *temp = curr;
                curr = curr->nextEdge;
                free(temp);
            }
        }
        free(map->vertList);
    }
    free(map);
    return newGraph;
}

void pvv_all(Graph *cities){
    for (int i = 0; i < cities->numVertex; i++){
        printf("Conexiones vertice [%c]:", cities->vertList[i].letter);
        Edge* curr = cities->vertList[i].edgeListHead;
        while (curr != NULL){
            printf(" %c(%d)", curr->destVert->letter, curr->cost);
            curr = curr->nextEdge;
        }
        printf("\n");
    }
}

