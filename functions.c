#include "functions.h"

int FileExists(const char *filename){
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

/**
 * Convierte cadena a numero entero positivo
 * usa ascii to integer (atoi) y convierte a positivo si es negativo
 * 
 * @param str:   cadena a convertir
 * @return:      numero entero positivo
 */
int CharToNum(char *str){
    int num = atoi(str);
    if(num < 0){
        num*= -1;
    }
    return num;
}

/** READ
 * Extrae las letras unicas de los vértices desde un archivo del grafo
 * Lee el archivo linea por linea, dado el formato (A, B, costo) y recolecta
 * las letras que no hayan sido agregadas antes.
 * 
 * @param filename: nombre del archivo de texto que contiene el grafo
 * @param size:     numero de vertices en el grafo
 * @return          arreglo dinamico de letras unicas encontradas en el archivo
 *                  NULL en caso de error
 * 
 * Nota: el arreglo retornado DEBE liberarse para evitar memory leaks (free())
 *       si (size > letras unicas), el arreglo contendra menos elementos 
 */
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

/**
 * busca una letra en un arreglo de letras de vertice
 * 
 * @param letters: arreglo de letras
 * @param vertex: letra a buscar
 * @param size: tamaño del arreglo
 * @return letra si se encuentra, -1 en caso contrario
 * 
 * Retorna la letra, no el indice, se espera char o -1
 */
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

/**
 * Se usa durante la creacion del grafo
 * asigna o encuentra un vertice para una letra dada
 * 
 * Busca en el grafo si ya existe un vertice con la letra newL
 * SI EXISTE    : guarda su indice en 'auxI'
 * SI NO EXISTE : Si se encuentra un slot vacio (letra '\0'), asigna la letra
 * 
 * @param newL:   letra a buscar o asignar (se modifica a '\0' si se encuentra)
 * @param i:      indice actual en el arreglo de vertices (vertList)
 * @param auxI:   puntero donde se guarda el indice del vertice encontrado/creado
 * @param map:    grafo donde buscar y asignar 
 */
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

/**
 * Crea y carga un grafo desde un archivo de texto
 * Pasos:
 * 1. verifica que el archivo de hecho exista
 * 2. lee y extrae vertices basado en las letras en el archivo
 * 3. asigna memoria para el grafo y los vertices
 * 4. inicializa el grafo, construyendo una tabla de mapeo letra->indice   
 * 5. crea las aristas bidirectionales entre vertices
 * 
 * @param size:     numero de vertices en el grafo
 * @param filename: nombre del archivo de texto que contiene el grafo
 * @return          puntero al grafo creado, NULL en caso de error
 */
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

/**
 * Verifica la seguridad de agregar un vertice al camino hamiltoniano
 * Debe cumplir:
 * 1. Si es la primera posición (pos == 0), siempre es seguro
 * 2. Debe existir una arista desde el ultimo vertice agregado
 * 
 * @param v:      vertice 'candidato' a agregar
 * @param pos:    posicion actual en el camino
 * @param path:   arreglo que almacena el camino actual
 * @param map:    puntero al grafo de ciudades
 * @return:       true si existe conexion desde el ultimo vertice, false si no
 */
bool isSafe(int v, int pos, int path[], Graph* map) {
    if (pos==0) return true;

    int lastVertex = path[pos - 1];
    int cost = getEdgeCost(map, lastVertex, v);

    return (cost != -1); /*hay conexion*/
}


/**
 * Realiza un retrocedimiento (backtracking) para encontrar el 
 * ciclo hamiltoniano de menor costo
 * @param map: puntero al grafo de ciudades
 * @param path: arreglo que almacena el camino actual
 * @param visited: arreglo booleano que indica si un vertice ha sido visitado
 * @param pos: posicion actual en el camino
 * @param currentCost: costo acumulado del camino actual
 * @param solution: puntero a la estructura que almacena la mejor solucion encontrada
 */
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

/**función para resolver el pvv
 * Encuentra el ciclo hamiltoniano de menor costo
 * usando backtracking (retroceso) con poda.
 * Imprime resultado en la consola al terminar
 * 
 * Pasos:
 * 1. Valida el grafo y asigna memoria
 * 2. Inicializa estructuras de retroceso
 * 3. Llama a pvvBactrack() desde vertice 0
 * 4. Muestra ruta optima y costo (o mensaje de no-existencia)
 * 5. Libera memoria asignada
 * 
 * @param map: puntero al grafo de ciudades
*/
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