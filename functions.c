#include "functions.h"

int FileExists(const char *filename){
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

/* READ */
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

/* ALL */
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

/* START */
int edgeCreation(int origin, int dest, int cost, Graph* map){
    Edge* newEdge = malloc(sizeof(Edge));
    if(newEdge == NULL){
        return 0;
    }
    newEdge->cost = cost;
    newEdge->destVert = &(map->vertList[dest]);

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
        (*newL) = '\0';
        return;
    }
}

Graph* createGraph(int size, const char* filename) {
    if (FileExists(filename) == 0){
        printf("ERROR: El archivo no existe\n");
        return NULL;
    }
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
        free(letters);
        return NULL;
    }

    Graph* map = malloc(sizeof(Graph));
    if (!map){
        free(letters);
        fclose(graph_text);
        fclose(graphdata);
        return NULL;
    }

    map->numVertex = size;
    map->vertList = malloc(size * sizeof(Vertex));

    if (!map->vertList) {
        free(map);
        free(letters);
        fclose(graph_text);
        fclose(graphdata);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        map->vertList[i].id = i;
        map->vertList[i].letter = '\0';
        map->vertList[i].edgeListHead = NULL;
    }

    int letter_to_index[256];
    for (int i = 0; i < 256; i++)
        letter_to_index[i] = -1;

    int next_index = 0;
    char v1, v2;
    int cost;

    while (fscanf(graph_text, " %c %c %d", &v1, &v2, &cost) == 3) {

        if (letter_to_index[(unsigned char)v1] == -1) {
            if (next_index >= size) {
                //printf("ERROR: Mas ciudades de las esperadas\n");
                free(letters);
                fclose(graph_text);
                fclose(graphdata);
                return map;
            }
            v1 = CheckLetterInGraph(letters, v1, size);
            letter_to_index[(unsigned char)v1] = next_index;
            map->vertList[next_index].letter = v1;
            next_index++;
        }

        if (letter_to_index[(unsigned char)v2] == -1) {
            if (next_index >= size) {
                //printf("ERROR: Mas ciudades de las esperadas\n");
                free(letters);
                fclose(graph_text);
                fclose(graphdata);
                return map;
            }
            v2 = CheckLetterInGraph(letters, v2, size);
            letter_to_index[(unsigned char)v2] = next_index;
            map->vertList[next_index].letter = v2;
            next_index++;
        }

        int idx1 = letter_to_index[(unsigned char)v1];
        int idx2 = letter_to_index[(unsigned char)v2];

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

    free(letters);
    fclose(graph_text);
    fclose(graphdata);
    return map;
}

//CICLO HAMILTONIANO

bool isSafe(int v, int pos, int path[], Graph* map) {
    Vertex* u = &map->vertList[path[pos - 1]];
    Vertex* nextV = &map->vertList[v];
    
    Edge* curr = u->edgeListHead;
    while (curr != NULL) { 
        if (curr->destVert->id == nextV->id) {
            return true;
        }
        curr = curr->nextEdge;
    }
    return false; 
}

bool hamiltonianCycleUtil(Graph* map, int path[], int pos) {
    int numVert = map->numVertex;

    if (pos == numVert) {
        Vertex* last = &map->vertList[path[pos - 1]];
        Vertex* first = &map->vertList[path[0]];
        
        Edge* curr = last->edgeListHead;
        while (curr != NULL) {
            if (curr->destVert->id == first->id) {
                return true; 
            }
            curr = curr->nextEdge;
        }
        return false; 
    }

    for (int v = 0; v < numVert; v++) { 
        if (isSafe(v, pos, path, map)) {
            bool alreadyUsed = false;
            for (int i = 0; i < pos; i++) {
                if (path[i] == v) {
                    alreadyUsed = true;
                    break;
                }
            }
            if (alreadyUsed) continue;

            path[pos] = v;
            if (hamiltonianCycleUtil(map, path, pos + 1))
                return true;
            path[pos] = -1;
        }
    }
    return false; 
}

void findHamiltonianCycle(Graph* map) {
    if (map == NULL || map->numVertex < 1) {
        printf("ERROR: El grafo no ha sido creado o es inválido.\n");
        return;
    }
    
    int numVert = map->numVertex;
    int *path = malloc(numVert * sizeof(int));
    if (!path) {
        printf("Error de asignación de memoria.\n");
        return;
    }

    for (int i = 0; i < numVert; i++) {
        path[i] = -1;
    }

    path[0] = 0; 
    
    printf("\n--- Verificando la existencia de una ruta viable ---\n");
    
    if (hamiltonianCycleUtil(map, path, 1) == false) {
        printf("No existe un camino que recorra todas las ciudades y regrese a la ciudad de origen.\n");
    } else {
        printf("Existe un camino que recorre todas las ciudades y regresa a la ciudad de origen.\n");
        printf("Uno de los ciclos hamiltonianos encontrados es: ");
        for (int i = 0; i < numVert; i++) {
            printf("%c -> ", map->vertList[path[i]].letter);
        }
        printf("%c\n", map->vertList[path[0]].letter);
    }

    free(path);
}