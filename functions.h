#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>
#include "structures.h"

Graph* createGraph(int size, const char* filename);
int FileExists(const char *filename);
void letterRecognition(char* newL, int i, int* auxI, Graph* map);
int edgeCreation(int origin, int dest, int cost, Graph* map);

