#ifndef FINDBLOCKS_H
#define FINDBLOCKS_H

#include <stdbool.h> // Include for using 'bool'
#include <stddef.h>  // Include for 'size_t'

// Represents the info about a block: number of vertexes and edges
typedef struct info_t
{
    unsigned int vertexes;
    unsigned int edges;
} info_t;

// Structure to represent an adjacency list node
typedef struct AdjListNode
{
    int dest;
    struct AdjListNode *next;
} AdjListNode;

// Structure to represent an adjacency list
typedef struct AdjList
{
    AdjListNode *head; // Pointer to the head node of the list
} AdjList;

// Structure to represent a graph
typedef struct Graph
{
    int V;          // Number of vertices
    AdjList *array; // Array of adjacency lists
} Graph;

/************************* GRAPH MANIPULATION **************************/
// Create a new adjacency list node
AdjListNode *newAdjListNode(int dest);

// Add an edge to an undirected graph
void addEdge(Graph *graph, int src, int dest);

// Create a graph with "V" vertexes
Graph *createGraph(int V);

// Print the adjacency list representation of the graph
void printGraph(Graph *graph);

// Print the block info found
void printBlocksInfo(info_t *blocksInfo, int numBlocks);

// Free the graph created
void freeGraph(Graph *graph);

// Read formatted input from file and create a graph
Graph *readFile(const char *filename);
/************************* GRAPH MANIPULATION **************************/

/************************* DFS PART *************************/
// DFS recursive to find articulation points
void DFSarticulationPoints(Graph *graph, int u, bool visited[], int disc[], int low[], int parent[], bool isArticulationPoint[], int *time);

// Find and print all articulation points
void findAllArticulationPoints(Graph *graph, bool *isArticulationPoint);
/************************* DFS PART *************************/

/************************* BFS PART *************************/
// BFS to find the vertex and edges in a block
void BFSblocks(Graph *graph, int startVertex, bool *visited, bool *isArticulationPoint, int *blockSize, bool *blockMembers);

// Find all blocks in the graph
void findAllBlocks(Graph *graph);
/************************* BFS PART *************************/
#endif