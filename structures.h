#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdio.h>
#include <stdlib.h>


typedef struct VertexStruct Vertex;
typedef struct EdgeStruct Edge;
typedef struct GraphStruct Graph;
typedef struct QueueStruct Queue;

struct QueueStruct{
    int *items;
    int front;
    int rear;
    int capacity;
};

struct VertexStruct {
    int id;
    char letter;
    //Arreglo dinamico para almacenar los bordes
    Edge* edgeListHead;
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

#endif