#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdio.h>
#include <stdlib.h>


typedef struct VertexStruct Vertex;
typedef struct EdgeStruct Edge;
typedef struct GraphStruct Graph;
typedef struct QueueStruct Queue;
typedef struct pvvSolution_ pvvSolution;

struct QueueStruct{
    int *items;
    int front;
    int rear;
    int capacity;
};

struct VertexStruct {
    int id;             // Indice numerico del vertice (0..n-1)
    char letter;        // identificador alfabetico del vertice
    Edge* edgeListHead; // lista enlazada de aristas salientes
};

//REpresenta un camino unidireccional entre el dueño y otro vertice.
struct EdgeStruct {
    Vertex* destVert;
    int cost;
    Edge* nextEdge;
};

struct GraphStruct {
    int numVertex;
    //Esta lista tendra tamaño fijo, asi que no hay 
    //necesidad de manejar el arreglo dinamico para conectar los vertices
    Vertex* vertList;
};

struct pvvSolution_{
    int *bestPath;   // Arreglo con la mejor ruta encontrada
    int bestCost;    // Costo total minimo encontrado
    int found;       // Flag: 1=solucion existe, 0=no existe
};

#endif