#include <stdio.h>
#include <stdlib.h>
#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct VertexStruct Vertex;
typedef struct EdgeStruct Edge;
typedef struct GraphStruct Graph;

struct VertexStruct {
    int id;
    char letter;

    //Dynamic array to store the list of edges.
    Edge* edgeListHead;
};

//Represents a one way path between the owner and another vertex.
struct EdgeStruct {
    Vertex* destVert;
    int cost;
    Edge* nextEdge;
};

struct GraphStruct {
    int numVertex;
    
    //This list will have a set size, so there is 
    //no need to handle dynamic arrays to connect all vertices
    Vertex* vertList;
};

#endif