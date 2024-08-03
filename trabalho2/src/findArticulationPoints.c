#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ANSI escape codes for colors and styles
char *reset = "\033[0m"; // Reset all styles
char *bold = "\033[1m";  // Bold text
char *red = "\033[31m";  // Red
char *white = "\033[37m";
char *purple = "\033[95m"; // Bright magenta (light purple)

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
        printf("\n");
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
void APUtil(Graph *graph, int u, bool visited[], int disc[], int low[], int parent[], bool isArticulationPoint[], int *time)
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
            APUtil(graph, v, visited, disc, low, parent, isArticulationPoint, time);
            // Check if the subtree rooted at v has a connection back to one of u's ancestors
            low[u] = (low[u] < low[v]) ? low[u] : low[v];
            // (1) u is an articulation point if it is not the root and low[v] >= disc[u]
            if (parent[u] != -1 && low[v] >= disc[u])
            {
                isArticulationPoint[u] = true;
            }
            // (2) u is an articulation point if it is the root and has two or more children
            if (parent[u] == -1 && children > 1)
            {
                isArticulationPoint[u] = true;
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
int findArticulationPoints(Graph *graph, bool *isArticulationPoint)
{
    printf("\n\n%s%s:: ARTICULATION POINTS ::%s\n", bold, purple, reset);
    bool *visited = (bool *)malloc(graph->V * sizeof(bool));
    int *disc = (int *)malloc(graph->V * sizeof(int));
    int *low = (int *)malloc(graph->V * sizeof(int));
    int *parent = (int *)malloc(graph->V * sizeof(int));
    int time = 0;
    int count = 0;

    // Initializion of arrays
    for (int i = 0; i < graph->V; i++)
    {
        parent[i] = -1;
        visited[i] = false;
        isArticulationPoint[i] = false;
    }
    // For each non visited graph, call DFS that find articulation points
    for (int i = 0; i < graph->V; i++)
    {
        if (!visited[i])
        {
            APUtil(graph, i, visited, disc, low, parent, isArticulationPoint, &time);
        }
    }

    printf("%s%sArticulation points in the graph:%s ", bold, white, reset);
    for (int i = 0; i < graph->V; i++)
    {
        if (isArticulationPoint[i] == true)
        {
            printf("%s%s%d%s ", bold, white, i + 1, reset); // Adjusted for 1-based indexing
            count++;
        }
    }
    // Freeing structs used
    free(visited);
    free(disc);
    free(low);
    free(parent);

    // Flag is true if the graph have at least one articulation point
    return count;
}

void DFSBlock(Graph *graph, int v, bool *visited, bool *isArticulationPoint, int *blockSize)
{
    // Print the current vertex where DFS is running (adjusting for 1-based indexing)
    printf("Current running DFS on vertex: %d\n", v + 1);
    visited[v] = true;
    (*blockSize)++;
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
    int numArticulationPoints = findArticulationPoints(graph, isArticulationPoint);
    int numBlocks = 0;
    int blockSize = 0;
    printf("\n\n%s%s:: BLOCKS ANALYSIS ::%s\n", bold, purple, reset);

    // For all vertices in the graph
    for (int i = 0; i < graph->V; i++)
    {
        if (isArticulationPoint[i] == true)
            printf("%s%sCurrent articulation point vertex: %d%s\n\n", bold, white, i + 1, reset);
        // If the vertex has not been visited or is not an articulation point
        if (!visited[i] && !isArticulationPoint[i])
        {
            // printf("numarticulatioon points: %d\n", numArticulationPoints);
            // If have articulation point consider it in the size
            blockSize = (numArticulationPoints > 0) ? 1 : 0;
            // Run DFS to verify how many vertex are in the block
            DFSBlock(graph, i, visited, isArticulationPoint, &blockSize);
            if (blockSize > 0) // Ensure to only count non-zero blocks
            {
                // Information of the block
                printf("%sBlock %d: Size %d%s\n\n", bold, numBlocks + 1, blockSize, reset);
                // Increase the number of blocks found
                numBlocks++;
                // Decrease the number of articulation points left to verify
                numArticulationPoints = (numArticulationPoints > 0) ? numArticulationPoints-1 : 0; 
            }
        }
        
    }
    printf("%s%sTotal number of blocks: %d%s\n", bold, purple, numBlocks, reset);
    // Freeing structs used
    free(visited);
    free(isArticulationPoint);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("%sUsage: %s <filename>%s\n", bold, argv[0], reset);
        return 1;
    }
    // Name of the file where the graph is located
    const char *filename = argv[1];
    // Read graph from file specified
    Graph *graph = readGraphFromFile(filename);

    if (graph != NULL)
    {
        printf("%s%s:: GRAPH READ ::%s\n", bold, purple, reset);
        printGraph(graph);
        // Find and analyse the blocks of the graph
        analyzeBlocks(graph);
        // Memory free
        freeGraph(graph);
        return 0;
    }
    printf("%s%sERROR => Could not initialize graph from file. Exiting...%s\n", bold, red, reset);
    return 1;
}
