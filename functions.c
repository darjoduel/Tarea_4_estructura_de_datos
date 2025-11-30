#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>
#include "structures.h"
#include "functions.h"


int FileExists(const char *filename){//verifica si un archivo existe
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

//Hay que modularizar esta función
Graph* createGraph(int size, const char* filename){
    char vert1, vert2;
    int cost, auxIndex1, auxIndex2;

    //--- Setup to read file ---
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("ERROR: No fué posible abrir el archivo\n");
        return NULL;
    }

    //--- Structure setup ---
    Graph* map = malloc(sizeof(Graph));
    if (map == NULL){
        printf("ERROR: No fué posible asignar memoria a grafo\n");
        return map;
    }
    map->numVertex = size;
    map->vertList = malloc(size * sizeof(Vertex));
    if (map->vertList == NULL){
        printf("ERROR: No fué posible asignar memoria a la lista de vértices\n");
        return NULL;
    }
    for (int i = 0; i < size; i++){
        map->vertList[i].edgeListHead = NULL;
        map->vertList[i].letter = '\0';
    }

    //--- Data scan loop ---
    while (fscanf(f, " %c %c %d", &vert1, &vert2, &cost) == 3) {

        //--- Letter recognition ---
        for (int i = 0; i < size; i++){
            letterRecognition(&vert1, i, &auxIndex1, map);
            letterRecognition(&vert2, i, &auxIndex2, map);
            if (vert1 == '\0' && vert2 == '\0') {
                break;
            }
        }
        //--- Edge definition ---
        if(!edgeCreation(auxIndex1, auxIndex2, cost, map)){
            printf("ERROR: No fué posible asignar memoria al enlace 1\n");
            return NULL;
        }
        if(!edgeCreation(auxIndex2, auxIndex1, cost, map)){
            printf("ERROR: No fué posible asignar memoria al enlace 2\n");
            return NULL;
        }
    }

    return map;
}

void letterRecognition(char* newL, int i, int* auxI, Graph* map){
    char currL = map->vertList[i].letter;
    
    if (currL == '\0'){
        if ((*newL) != '\0'){ 
            map->vertList[i].id = i;
            map->vertList[i].letter = (*newL);
            (*auxI) = i;
            (*newL) = '\0';
            return;
        }

    } else if (currL == (*newL)) {
        (*auxI) = i;
        (*newL) = '\0'; //newL marks itself as consumed
        return;
    }
}

int edgeCreation(int origin, int dest, int cost, Graph* map){
    //Edge creation and definition
    Edge* newEdge = malloc(sizeof(Edge));
    if(newEdge == NULL) return 0;
    newEdge->cost = cost;
    newEdge->destVert = &(map->vertList[dest]);

    //Insert at the start of the array
    newEdge->nextEdge = map->vertList[origin].edgeListHead;
    map->vertList[origin].edgeListHead = newEdge;

    return 1;
}