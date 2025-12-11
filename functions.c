#include "functions.h"

int FileExists(const char *filename){
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

int CharToNum(char *str){// convierte cadena a numero entero positivo
    int num = atoi(str);
    if(num < 0){
        num*= -1;
    }
    return num;
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


    char *letters = CheckGraphText(filename, size);
    if (letters == NULL) {
        printf("ERROR: No fue posible leer las letras del grafo\n");
        fclose(graph_text);
        free(letters);
        return NULL;
    }

    Graph* map = malloc(sizeof(Graph));
    if (!map){
        free(letters);
        fclose(graph_text);
        return NULL;
    }

    map->numVertex = size;
    map->vertList = malloc(size * sizeof(Vertex));

    if (!map->vertList) {
        free(map);
        free(letters);
        fclose(graph_text);
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
    return map;
}

//CICLO HAMILTONIANO

/*obtiene el costo de una arista entre dos vetrices*/
int getEdgeCost(Graph* map, int from, int to)
{
    Vertex* fromVertex = &map->vertList[from];
    Edge* curr = fromVertex->edgeListHead;

    while (curr != NULL)
    {
        if (curr->destVert->id == to)
        {
            return curr->cost;
        }
        curr = curr->nextEdge;
    }
    return -1; /*indica que no existe conexión*/
}

bool isSafe(int v, int pos, int path[], Graph* map) {
    if (pos==0) return true;

    int lastVertex = path[pos - 1];
    int cost = getEdgeCost(map, lastVertex, v);

    return (cost != -1); /*hay conexion*/
}

/*retrocedimiento para encontrar el ciclo de menor costo*/
void pvvBacktrack(Graph* map, int path[], bool visited[], int pos, 
                  int currentCost, pvvSolution* solution)
{
    int numVert = map->numVertex;
    
    /*caso base: se han visitado todas las ciudades*/
    if (pos == numVert)
    {
        int lastVertex  = path[pos - 1];
        int firstVertex = path[0];
        int returnCost  = getEdgeCost(map, lastVertex, firstVertex);

        if (returnCost != -1)
        {
            int totalCost = currentCost + returnCost;
            if (!solution->found || totalCost < solution->bestCost)
            {
                solution->found = true;
                solution->bestCost = totalCost;
                /*copiar el camino*/
                for (int i = 0; i < numVert; i++)
                {
                    solution->bestPath[i] = path[i];
                }
            }
        }
        return;
    }
    /*probar con cada vertice no visitado*/
    for (int v = 0; v < numVert; v++)
    {
        if (!visited[v] && isSafe(v, pos, path, map))
        {/*calcular el costo para ir al proximo vertice*/
            int edgeCost = 0;
            if (pos > 0){
                edgeCost = getEdgeCost(map, path[pos - 1], v);
            }

            /* Poda: si se supera el mejor costo calculado, no seguir*/
            if (solution->found && (currentCost + edgeCost >= solution->bestCost)){
                continue; 
            }
            /*marcar como visitado el vertice*/
            visited[v] = true;
            path[pos] = v;

            /*recursion !!!!!!*/
            pvvBacktrack(map, path, visited, pos + 1, currentCost + edgeCost, solution);

            /*bakctracking*/
            visited[v] = false;
            path[pos] = -1;
        }
    }
}

/*función para resolver el pvv*/
void solvePVV(Graph* map)
{
    if (map == NULL || map -> numVertex < 1)
    {
        printf("ERROR: El grafo no ha sido creado o es invalido.\n");
        return;
    }

    int numVert = map->numVertex;
    /*inicializar estructuras*/
    int *path = malloc(numVert * sizeof(int));
    bool *visited = malloc(numVert * sizeof(bool));
    pvvSolution solution;
    solution.bestPath = malloc(numVert * sizeof(int));
    solution.bestCost = INT_MAX;
    solution.found = false;

    if (!path || !visited || !solution.bestPath)
    {
        printf("Error de asignacion de memoria.\n");
        free(path);
        free(visited);
        free(solution.bestPath);
        return;
    }
    /*inicializar*/
    for (int i = 0; i < numVert; i++)
    {
        path[i] = -1;
        visited[i] = false;
    }
 
    /*comenzar desde el primer vertice*/
    path[0] = 0;
    visited[0] = true;

    printf("\n=== Verificando la existencia de una ruta viable ===\n");
    /*ejecutar backtracking*/
    pvvBacktrack(map, path, visited, 1, 0, &solution);

    /*prints de resultados*/
    if (!solution.found){
        printf("No existe un camino que recorra todas las ciudades y regrese al origen.\n");
    } else {
        printf("Existe un camino viable que recorre todas las ciudades y vuelve al origen.\n");
        printf("Ruta a seguir: ");
        for (int i = 0; i < numVert; i++){
            printf("%c ", map->vertList[solution.bestPath[i]].letter);
        }
        printf("%c\n", map->vertList[solution.bestPath[0]].letter);
        printf("Costo total del viaje: %d\n", solution.bestCost);
    }
    /*liberar memoria*/
    free(path);
    free(visited);
    free(solution.bestPath);
}

void findHamiltonianCycle(Graph* map){
    solvePVV(map);
}