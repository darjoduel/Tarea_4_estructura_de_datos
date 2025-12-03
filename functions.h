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
/*READ*/
char *CheckGraphText(const char *filename, int size){
    FILE *graph_text = fopen(filename, "a+");
    if (!graph_text){ 
        return NULL;
    }

    char vert1, vert2;
    int cost;

    char *letters = calloc(size, sizeof(char));
    int count = 0;

    while (fscanf(graph_text, " %c %c %d", &vert1, &vert2, &cost) == 3){
        
        int exists=0;
        for (int i = 0; i < count; i++){
            if (letters[i] == vert1){
                exists=1;
                break;
            }
        }

        if (!exists && count < size){
            letters[count++] = vert1;
        }

        exists=0;
        for (int i = 0; i < count; i++){
            if (letters[i] == vert2){
                exists = 1;
                break;
            }
        }

        if (!exists && count < size){
            letters[count++] = vert2;
        }
        if (count == size){
            break;
        } 
    }

    /*for (int i = 0; i < size; i++){
        if (letters[i] == '\0') {
            break;
        }
        printf("%c ", letters[i]);
    }*/
    //free(letters);
    fclose(graph_text);
    return letters;
}

int CheckLetterInGraph(char *letters, char vertex, int size){
    for (int i = 0; i < size; i++){
        if (letters[i] == vertex){
            return vertex;
        }
    }
    return -1;
}
/*int ReadGraphText(const char *filename, char *letter, int size){}*/
/*ALL*/
void ShowGraph(Graph *cities){
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
/*START*/
int edgeCreation(int origin, int dest, int cost, Graph* map){
    //Edge creation and definition
    Edge* newEdge = malloc(sizeof(Edge));
    if(newEdge == NULL){
        return 0;
    }
    newEdge->cost = cost;
    newEdge->destVert = &(map->vertList[dest]);

    //Insert at the start of the array
    newEdge->nextEdge = map->vertList[origin].edgeListHead;
    map->vertList[origin].edgeListHead = newEdge;

    return 1;
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
//Hay que modularizar esta función
Graph* createGraph(int size, const char* filename) {

    FILE* graph_text = fopen(filename, "r");
    if (graph_text == NULL) {
        printf("ERROR: No fue posible abrir el archivo\n");
        return NULL;
    }
    FILE *graphdata = fopen("graphdata", "r");
    if (graphdata == NULL) {
        printf("ERROR: No fue posible abrir el archivo graphdata\n");
        fclose(graph_text);
        return NULL;
    }

    char *letters = CheckGraphText(filename, size);
    if (letters == NULL) {
        printf("ERROR: No fue posible leer las letras del grafo\n");
        fclose(graph_text);
        fclose(graphdata);
        return NULL;
    }

    // Crear grafo con n vértices
    Graph* map = malloc(sizeof(Graph));
    if (!map){
        return NULL;
    }

    map->numVertex = size;
    map->vertList = malloc(size * sizeof(Vertex));

    if (!map->vertList) {
        free(map);
        return NULL;
    }

    // Inicializar todos los vértices
    for (int i = 0; i < size; i++) {
        map->vertList[i].id = i;
        map->vertList[i].letter = '\0';
        map->vertList[i].edgeListHead = NULL;
    }

    // Mapa para traducir letras A..Z a índices
    int letter_to_index[256];
    for (int i = 0; i < 256; i++)
        letter_to_index[i] = -1;

    int next_index = 0;
    char v1, v2;
    int cost;

    while (fscanf(graph_text, " %c %c %d", &v1, &v2, &cost) == 3) {

        // Asignar índice a v1 si no existe
        if (letter_to_index[(unsigned char)v1] == -1) {
            if (next_index >= size) {
                printf("ERROR: Más ciudades de las esperadas\n");
                fclose(graph_text);
                return map;
            }
            v1 = CheckLetterInGraph(letters, v1, size);
            letter_to_index[(unsigned char)v1] = next_index;
            map->vertList[next_index].letter = v1;
            next_index++;
        }

        // Asignar índice a v2 si no existe
        if (letter_to_index[(unsigned char)v2] == -1) {
            if (next_index >= size) {
                printf("ERROR: Más ciudades de las esperadas\n");
                fclose(graph_text);
                return map;
            }
            v2 = CheckLetterInGraph(letters, v2, size);
            letter_to_index[(unsigned char)v2] = next_index;
            map->vertList[next_index].letter = v2;
            next_index++;
        }

        int idx1 = letter_to_index[(unsigned char)v1];
        int idx2 = letter_to_index[(unsigned char)v2];

        // Crear aristas en ambos sentidos
        Edge* e1 = malloc(sizeof(Edge));
        e1->destVert = &map->vertList[idx2];
        e1->cost = cost;
        e1->nextEdge = map->vertList[idx1].edgeListHead;
        map->vertList[idx1].edgeListHead = e1;

        Edge* e2 = malloc(sizeof(Edge));
        e2->destVert = &map->vertList[idx1];
        e2->cost = cost;
        e2->nextEdge = map->vertList[idx2].edgeListHead;
        map->vertList[idx2].edgeListHead = e2;
    }

    fclose(graph_text);
    return map;
}

