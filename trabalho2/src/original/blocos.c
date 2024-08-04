#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Include for using 'bool'
#include <stddef.h>  // Include for 'size_t'
#include "blocos.h"

// #define GRAPH_INFO 1

// ANSI escape codes for colors and styles
const char *reset = "\033[0m";   // Reset all styles
const char *bold = "\033[1m";    // Bold text
const char *red = "\033[31m";    // Red
const char *white = "\033[37m";  // White
const char *purple = "\033[95m"; // Bright magenta (light purple)

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

// Add an aresta to an undirected graph
void addEdge(Graph *graph, int src, int dest);

// Create a graph with "V" vertices
Graph *createGraph(int V);

#ifdef GRAPH_INFO
// Print the adjacency list representation of the graph
void printGraph(Graph *graph);

// Print the block info found
void printBlocksInfo(info_t *blocksInfo, int numBlocks);
#endif

// Free the graph created
void freeGraph(Graph *graph);

// Read formatted input from stdin and create a graph
Graph *readInput(void);
/************************* GRAPH MANIPULATION **************************/

/************************* DFS PART *************************/
// DFS recursive to find articulation points
void DFSarticulationPoints(Graph *graph, int u, bool visited[], int disc[], int low[], int parent[], bool isArticulationPoint[], int *time);

// Find and print all articulation points
void findAllArticulationPoints(Graph *graph, bool *isArticulationPoint);
/************************* DFS PART *************************/

/************************* BFS PART *************************/
// BFS to find the vertices and arestas in a block
void BFSblocks(Graph *graph, int startvertices, bool *visited, bool *isArticulationPoint, int *blockSize, bool *blockMembers);

// Find all blocks in the graph
info_t* findAllBlocks(Graph *graph, unsigned int *num_blocos);
/************************* BFS PART *************************/

AdjListNode *newAdjListNode(int dest)
{
    // Allocate memory for a new adjacency list node
    AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    // Set the destination vertices
    newNode->dest = dest;
    // Initialize the next pointer
    newNode->next = NULL;
    // Return the new node
    return newNode;
}

void addEdge(Graph *graph, int src, int dest)
{
    // Add an edge from src to dest
    AdjListNode *newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    // Since the graph is undirected, add an edge from dest to src as well
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

Graph *createGraph(int V)
{
    // Allocate memory for the graph structure
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    // Set the number of vertices in the graph
    graph->V = V;
     // Allocate memory for the array of adjacency lists
    // Each vertice will have its own adjacency list
    graph->array = (AdjList *)malloc((size_t)V * sizeof(AdjList));
    // Loop over all vertices
    for (int i = 0; i < V; ++i)
        // Initialize each adjacency list as empty by setting its head to NULL
        graph->array[i].head = NULL;
    // Return the newly created graph structure
    return graph;
}

#ifdef GRAPH_INFO
void printGraph(Graph *graph)
{
    // Print header for graph visualization
    printf("%s%s:: GRAPH READ ::%s\n", bold, purple, reset);
    // Loop through each vertex in the graph
    for (int v = 0; v < graph->V; ++v)
    {
        AdjListNode *pCrawl = graph->array[v].head;
        // Print the adjacency list for vertex v (1-based index)
        printf("%s%sAdjacency list of vertex %s%d: (head)%s ", bold, white, purple, v + 1, reset);
        while (pCrawl)
        {
            // Print the destination vertex of each edge (1-based index)
            printf("-> %s%d%s ", bold, pCrawl->dest + 1, reset);
            pCrawl = pCrawl->next;
        }
        printf("\n"); // New line after each adjacency list
    }
}

void printBlocksInfo(info_t *blocksInfo, int numBlocks)
{
    printf("\n%s%s:: BLOCO INFO ::%s\n", bold, purple, reset);
    for (int i = 0; i < numBlocks; i++)
        if (blocksInfo[i].vertexes == 1 && blocksInfo[i].edges == 1)
            printf("%s%sBlock %d: %d Vertex, %d Edge %s\n", bold, white, i + 1, blocksInfo[i].vertexes, blocksInfo[i].edges, reset);
        else if (blocksInfo[i].vertexes == 1)
            printf("%s%sBlock %d: %d Vertex, %d Edges %s\n", bold, white, i + 1, blocksInfo[i].vertexes, blocksInfo[i].edges, reset);
        else if (blocksInfo[i].edges == 1)
            printf("%s%sBlock %d: %d Vertexes, %d Edge %s\n", bold, white, i + 1, blocksInfo[i].vertexes, blocksInfo[i].edges, reset);
        else
            printf("%s%sBlock %d: %d Vertexes, %d Edges %s\n", bold, white, i + 1, blocksInfo[i].vertexes, blocksInfo[i].edges, reset);
    printf("\n");
    // Summarize the blocks info
    printf("%s%sTOTAL NUMBER OF BLOCKS IN THE GRAPH: %d%s\n", bold, purple, numBlocks, reset);
}
#endif

Graph *readInput(void)
{
    // Read the total number of vertices in the graph
    int V;
    scanf("%d", &V);
    // Create the graph with the number of vertices read
    Graph *graph = createGraph(V);
    // Populate the Adjacency List with the arestas
    int src, dest;
    while (scanf("%d %d", &src, &dest) != EOF) {
        addEdge(graph, src - 1, dest - 1); // Adjust for 0-based indexing in C
    }
    // Return the graph created
    return graph;
}

void DFSarticulationPoints(Graph *graph, int u, bool visited[], int disc[], int low[], int parent[], bool isArticulationPoint[], int *time)
{
    // Number of children in the DFS tree
    int children = 0;
    // Mark the current node as visited
    visited[u] = true;
    // Initialize discovery time and low value
    disc[u] = low[u] = ++(*time);
    // Set "u" as root of the DFS tree
    AdjListNode *pCrawl = graph->array[u].head;
    // For each neighbor of the root...
    while (pCrawl != NULL)
    {
        // Current adjacent vertices
        int v = pCrawl->dest;
        // If "v" is not visited, then it is a tree aresta
        if (!visited[v])
        {
            children++;
            // Set "u" as parent of "v"
            parent[v] = u;
            // Recursive call for vertices v
            DFSarticulationPoints(graph, v, visited, disc, low, parent, isArticulationPoint, time);
            // Check if the subtree rooted at "v" has a connection back to one of u's ancestors
            low[u] = (low[u] < low[v]) ? low[u] : low[v];
            // (1) "u" is an articulation point if it is the root and has two or more children
            if (parent[u] == -1 && children > 1)
                isArticulationPoint[u] = true;
            // (2) "u" is an articulation point if it is not the root and low value(v) >= discovery time(u)
            if (parent[u] != -1 && low[v] >= disc[u])
                isArticulationPoint[u] = true;
        }
        else if (v != parent[u])
            low[u] = (low[u] < disc[v]) ? low[u] : disc[v]; // Update low(u) to the minimum of low(u) and disc(v) for back(<-) aresta v-u
        pCrawl = pCrawl->next;                              // Move to the next adjacent vertices
    }
}

void findAllArticulationPoints(Graph *graph, bool *isArticulationPoint)
{
    #ifdef GRAPH_INFO
    // Print header for articulation points
    printf("%s%s:: ARTICULATION POINTS ::%s\n", bold, purple, reset);
    #endif

    // Allocate memory for auxiliary arrays
    bool *visited = (bool *)malloc((size_t)graph->V * sizeof(bool));
    int *disc = (int *)malloc((size_t)graph->V * sizeof(int));
    int *low = (int *)malloc((size_t)graph->V * sizeof(int));
    int *parent = (int *)malloc((size_t)graph->V * sizeof(int));
    // Initialize time
    int time = 0;
    // Initialization of arrays
    for (int i = 0; i < graph->V; i++)
    {
        parent[i] = -1;                 // Initialize parent array
        visited[i] = false;             // Mark all vertices as not visited
        isArticulationPoint[i] = false; // vertices at position "i" is not an articulation point
    }
    // For each vertices, if not visited, call DFS to find articulation points
    for (int i = 0; i < graph->V; i++)
        if (!visited[i])
            DFSarticulationPoints(graph, i, visited, disc, low, parent, isArticulationPoint, &time);

    #ifdef GRAPH_INFO
    // Print all articulation points found
    printf("%s%sArticulation points in the graph:%s ", bold, white, reset);
    for (int i = 0; i < graph->V; i++)
        if (isArticulationPoint[i] == true)
            printf("%s%s%d%s ", bold, white, i + 1, reset); // Adjusted for 1-based indexing
    #endif

    // Freeing allocated memory
    free(visited);
    free(disc);
    free(low);
    free(parent);
}

void BFSblocks(Graph *graph, int startVertices, bool *visited, bool *isArticulationPoint, int *blockSize, bool *blockMembers)
{
    // Initialize a queue for BFS
    int *queue = (int *)malloc((size_t)graph->V * sizeof(int));
    int front = 0;
    int rear = 0;

    // Start with the vertices "startVertices"
    queue[rear++] = startVertices;
    // Mark vertices "startVertices" as visited
    visited[startVertices] = true;
    // Mark vertices "startVertices" as part of this block
    blockMembers[startVertices] = true;
    // Added vertices "startVertices" to the block
    (*blockSize)++;

    #ifdef GRAPH_INFO
    // Print the vertices added to the block
    printf("vertices %d added to the block\n", startVertices + 1); // +1 for 1-based indexing
    #endif

    // While has not reach rear of the queue
    while (front < rear)
    {
        // Get the first vertices in queue
        int current = queue[front++];
        // For all it's neighbours...
        AdjListNode *pCrawl = graph->array[current].head;
        while (pCrawl != NULL)
        {
            // Get the neighbour of the vertices
            int neighbour = pCrawl->dest;
            if (!visited[neighbour])
            {
                // Mark neighbour as visited
                visited[neighbour] = true;
                // Mark neighbour as part of this block
                blockMembers[neighbour] = true;

                #ifdef GRAPH_INFO
                // Print the vertices added to the block
                printf("vertices %d added to the block\n", neighbour + 1); // +1 for 1-based indexing
                #endif

                // If neighbour it's not an articulation point, continue exploring
                if (!isArticulationPoint[neighbour])
                    queue[rear++] = neighbour;
                // Add neighbour to the block
                (*blockSize)++;
            }
            // Get the next vertices
            pCrawl = pCrawl->next;
        }
    }
    // Free the allocated queue
    free(queue);
}

void freeGraph(Graph *graph)
{
    // Loop through each vertices
    for (int i = 0; i < graph->V; ++i)
    {
        AdjListNode *current = graph->array[i].head;
        // Free all nodes in the adjacency list of the current vertices
        while (current != NULL)
        {
            AdjListNode *temp = current;
            current = current->next;
            free(temp); // Free the current node
        }
    }
    free(graph->array); // Free the array of adjacency lists
    free(graph);        // Free the graph structure
}

info_t* findAllBlocks(Graph *graph, unsigned int *num_blocos)
{
    // Array to track vertices visited
    bool *visited = (bool *)malloc((size_t)graph->V * sizeof(bool));
    // Array to track articulation points
    bool *isArticulationPoint = (bool *)malloc((size_t)graph->V * sizeof(bool));
    // Array to track block members
    bool *blockMembers = (bool *)malloc((size_t)graph->V * sizeof(bool));
    // Array to store information of all blocks
    info_t *blocksInfo = (info_t *)malloc((size_t)graph->V * sizeof(info_t));
    // Initialize graph as not visited
    for (int i = 0; i < graph->V; i++)
    {
        visited[i] = false;
        blockMembers[i] = false;
    }
    // Find all articulation points in the graph
    findAllArticulationPoints(graph, isArticulationPoint);

    #ifdef GRAPH_INFO
    // Print header for block analysis
    printf("\n\n%s%s:: BLOCKS ANALYSIS ::%s\n", bold, purple, reset);
    #endif

    // Initialize number of blocks found
    unsigned int numBlocks = 0;
    // Iterate over all vertices in the graph
    for (int i = 0; i < graph->V; i++)
    {
        // If the vertices is not visited or is an articulation point, start a new block
        if (!visited[i] || isArticulationPoint[i])
        {
            // Initialize size of the block
            int blockSize = 0;
            // Initialize number of arestas for the current block
            int numArestas = 0;
            // Initialize blockMembers array for the current block
            for (int j = 0; j < graph->V; j++)
                blockMembers[j] = false;
            // BFS to find the block size and mark block members
            BFSblocks(graph, i, visited, isArticulationPoint, &blockSize, blockMembers);
            // For all vertices in the graph
            for (int v = 0; v < graph->V; v++)
            {
                // If the vertices "v" belongs to the current block
                if (blockMembers[v])
                {
                    AdjListNode *pCrawl = graph->array[v].head;
                    // Iterate for each neighbor of "v"
                    while (pCrawl != NULL)
                    {
                        // If the neighbor of "v" is in the block
                        if (blockMembers[pCrawl->dest])
                            numArestas++; // Add the aresta to the block
                        // Get the next neighbor
                        pCrawl = pCrawl->next;
                    }
                }
            }
            // If it's a valid block (has at least 1 vertices)
            if (blockSize > 0)
            {
                // Since every aresta is counted twice (v-u, u-v) in an undirected graph, divide by 2
                numArestas /= 2;
                // Store the block information
                blocksInfo[numBlocks].vertices = (unsigned int)blockSize;
                blocksInfo[numBlocks].arestas = (unsigned int)numArestas;

                #ifdef GRAPH_INFO
                // Print the block information found
                printf("%sFound Block %d: Number of vertices %d, Number of arestas: %d%s\n\n", bold, numBlocks + 1, blockSize, numArestas, reset);
                #endif

                // Increase the number of blocks found
                numBlocks++;
            }
        }
    }
    (*num_blocos) = numBlocks;

    #ifdef GRAPH_INFO
    printBlocksInfo(blocksInfo, numBlocks);
    #endif

    // Free allocated memory
    free(visited);
    free(isArticulationPoint);
    free(blockMembers);
    freeGraph(graph);

    return blocksInfo;
}

info_t *info_blocos(unsigned int *num_blocos) {
    Graph *graph = readInput();

    if (graph != NULL)
    {
        #ifdef GRAPH_INFO
        printGraph(graph);
        printf("\n");
        #endif

        return findAllBlocks(graph, num_blocos);
    }
    printf("%s%sERROR => Could not create Graph.%s\n", bold, red, reset);
    exit(1);
}