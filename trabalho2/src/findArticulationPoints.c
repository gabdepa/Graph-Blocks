#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

// Function to create a new adjacency list node
AdjListNode *newAdjListNode(int dest)
{
    AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with V vertices
Graph *createGraph(int V)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList *)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

// Function to add an edge to an undirected graph
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

// Function to print the adjacency list representation of the graph
void printGraph(Graph *graph)
{
    for (int v = 0; v < graph->V; ++v)
    {
        AdjListNode *pCrawl = graph->array[v].head;
        printf("Adjacency list of vertex %d\n head ", v + 1); // Adjust for 1-based indexing
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest + 1); // Adjust for 1-based indexing
            pCrawl = pCrawl->next;
        }
        printf("\n\n");
    }
}

// Function to free the graph
void freeGraph(Graph *graph)
{
    for (int i = 0; i < graph->V; ++i)
    {
        AdjListNode *current = graph->array[i].head;
        while (current != NULL)
        {
            AdjListNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->array);
    free(graph);
}

// Function to read graph from file and create graph
Graph *readGraphFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", filename);
        return NULL;
    }

    int V;
    fscanf(file, "%d", &V);
    Graph *graph = createGraph(V);

    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) != EOF)
    {
        addEdge(graph, src - 1, dest - 1); // Adjust for 0-based indexing
    }

    fclose(file);
    return graph;
}

// Recursive function to find articulation points using DFS
void APUtil(Graph *graph, int u, bool visited[], int disc[], int low[], int parent[], bool ap[], int *time)
{
    int children = 0;
    visited[u] = true;
    disc[u] = low[u] = ++(*time); // Initialize discovery time and low value

    AdjListNode *pCrawl = graph->array[u].head;
    while (pCrawl != NULL)
    {
        int v = pCrawl->dest; // Current adjacent vertex
        if (!visited[v])
        {
            children++;
            parent[v] = u;
            APUtil(graph, v, visited, disc, low, parent, ap, time);
            // Check if the subtree rooted at v has a connection back to one of u's ancestors
            low[u] = (low[u] < low[v]) ? low[u] : low[v];
            // (1) u is an articulation point if it is not the root and low[v] >= disc[u]
            if (parent[u] != -1 && low[v] >= disc[u])
            {
                ap[u] = true;
            }
            // (2) u is an articulation point if it is the root and has two or more children
            if (parent[u] == -1 && children > 1)
            {
                ap[u] = true;
            }
        }
        else if (v != parent[u])
        {
            // Update low[u] for back edge v -- u
            low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
        }
        pCrawl = pCrawl->next;
    }
}

// Function to find and print all articulation points
void findArticulationPoints(Graph *graph, bool *ap)
{
    bool *visited = (bool *)malloc(graph->V * sizeof(bool));
    int *disc = (int *)malloc(graph->V * sizeof(int));
    int *low = (int *)malloc(graph->V * sizeof(int));
    int *parent = (int *)malloc(graph->V * sizeof(int));
    // bool *ap = (bool *)malloc(graph->V * sizeof(bool));
    int time = 0;

    for (int i = 0; i < graph->V; i++)
    {
        parent[i] = -1;
        visited[i] = false;
        ap[i] = false;
    }

    for (int i = 0; i < graph->V; i++)
    {
        if (!visited[i])
        {
            APUtil(graph, i, visited, disc, low, parent, ap, &time);
        }
    }

    printf("Articulation points in the graph:\n");
    for (int i = 0; i < graph->V; i++)
    {
        if (ap[i] == true)
        {
            printf("%d\n", i + 1); // Adjusted for 1-based indexing
        }
    }

    free(visited);
    free(disc);
    free(low);
    free(parent);
    // free(ap);
}

void DFSBlock(Graph *graph, int v, bool *visited, bool *isArticulationPoint, int *blockSize)
{
    visited[v] = true;
    (*blockSize)++;
    // Print the current vertex (adjusting for 1-based indexing)
    printf("Current v: %d\n", v + 1);
    AdjListNode *pCrawl = graph->array[v].head;
    while (pCrawl != NULL)
    {
        int u = pCrawl->dest;
        if (!visited[u] && !isArticulationPoint[u])
        {
            DFSBlock(graph, u, visited, isArticulationPoint, blockSize);
        }
        pCrawl = pCrawl->next;
    }
}

void analyzeBlocks(Graph *graph)
{
    bool *visited = (bool *)malloc(graph->V * sizeof(bool));
    bool *isArticulationPoint = (bool *)malloc(graph->V * sizeof(bool));

    // Initialize arrays
    for (int i = 0; i < graph->V; i++)
    {
        visited[i] = false;
    }

    // Find articulation points
    findArticulationPoints(graph, isArticulationPoint);

    int numBlocks = 0;
    printf("\nBLOCKS ANALYSIS:\n");

    // Traverse all vertices
    for (int i = 0; i < graph->V; i++)
    {
        if (isArticulationPoint[i] == true)
            printf("Current articulation point vertex: %d\n\n", i + 1);
        if (!visited[i] && !isArticulationPoint[i])
        {
            int blockSize = 1; // Account the articulation point
            DFSBlock(graph, i, visited, isArticulationPoint, &blockSize);
            if (blockSize > 0) // Ensure to only count non-zero blocks
            {
                printf("Block %d: Size %d\n\n", numBlocks + 1, blockSize);
                numBlocks++;
            }
        }
    }

    printf("Total number of blocks: %d\n", numBlocks);

    free(visited);
    free(isArticulationPoint);
}

int main()
{
    Graph *graph = readGraphFromFile("inputs/random.txt");
    // Alocação do array para armazenar pontos de articulação
    // bool *isArticulationPoint = (bool *)malloc(graph->V * sizeof(bool));
    if (graph != NULL)
    {
        printGraph(graph);
        // findArticulationPoints(graph, isArticulationPoint);
        // Análise dos blocos do grafo
        analyzeBlocks(graph);
        // Liberação de memória
        // free(isArticulationPoint);
        freeGraph(graph);
    }
    return 0;
}
