#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>
#include "structures.h"

int FileExists(const char *filename){//verifica si un archivo existe
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

void letterRecognition(char* newL, int i, int* auxI, Graph* map){
    char currL = map->vertList[i].letter;
    
    if (currL == '\0'){
        if ((*newL) != '\0'){ 
            //printf(" New letter found, new vertex will be defined \n");
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

//Hay que modularizar esta función
Graph* createGraph(int size, const char* filename){
    //Setup to read file
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("ERROR: No fué posible abrir el archivo\n");
        return;
    }

    char vert1, vert2;
    int cost, auxIndex1, auxIndex2;
    
    //Structure setup
    Graph map;
    map.numVertex = size;
    map.vertList = malloc(size * sizeof(Vertex));
    //"""COMPROBAR QUE SE HAYA LOCALIZADO BIEN LA MEMORIA"""

    for (int i = 0; i < size; i++){
        map.vertList[i].edgeListHead = NULL;
        map.vertList[i].letter = '\0';
    }

    //Data scan loop
    while (fscanf(f, " %c %c %d", &vert1, &vert2, &cost) == 3) {
        printf("Reading %c <-> %c | Cost:%d\n", vert1, vert2, cost);

        //Letter recognition
        for (int i = 0; i < size; i++){
            
            letterRecognition(&vert1, i, &auxIndex1, &map);
            letterRecognition(&vert2, i, &auxIndex2, &map);

            if (vert1 == '\0' && vert2 == '\0') break;
        }

        printf("Index 1: %d\n", auxIndex1);
        printf("Index 2: %d\n", auxIndex2);
        
        // --- IMPLEMENTAR CREACIÓN DE EDGE PARA INDEX 1 E INDEX 2 --- 
        
    }

}

