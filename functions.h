#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>
#include "structures.h"

//DECLARACIONES DE FUNCIONES EXISTENTES
int FileExists(const char *filename);
char *CheckGraphText(const char *filename, int size);
int CheckLetterInGraph(char *letters, char vertex, int size);
void ShowGraph(Graph *cities);
int edgeCreation(int origin, int dest, int cost, Graph* map);
void letterRecognition(char* newL, int i, int* auxI, Graph* map);
Graph* createGraph(int size, const char* filename);

//DECLARACIONES PARA EL CICLO HAMILTONIANO
bool isSafe(int v, int pos, int path[], Graph* map);
bool hamiltonianCycleUtil(Graph* map, int path[], int pos);
void findHamiltonianCycle(Graph* map);