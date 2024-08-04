#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ANSI escape codes for colors and styles
char *reset = "\033[0m"; // Reset all styles
char *bold = "\033[1m";  // Bold text
char *red = "\033[31m";  // Red
char *white = "\033[37m"; // White
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

// Create a new adjacency list node
AdjListNode *newAdjListNode(int dest)
{
    // Allocate memory for a new adjacency list node
    AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    // Set the destination vertex
    newNode->dest = dest;
    // Initialize the next pointer
    newNode->next = NULL;
    // Return the new node
    return newNode;
}

// Add an edge to an undirected graph
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

// Create a graph with "V" vertexs
Graph *createGraph(int V)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList *)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

// Print the adjacency list representation of the graph
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

// Free the graph
void freeGraph(Graph *graph)
{
    // Loop through each vertex
    for (int i = 0; i < graph->V; ++i)
    {
        AdjListNode *current = graph->array[i].head;
        // Free all nodes in the adjacency list of the current vertex
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

// Read formatted input from file and create a graph
Graph *readFile(const char *filename)
{
    // Try to open the file read-only
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // Could not open the file
        printf("%s%sERROR => Could not open file %s\n%s", bold, red, filename, reset);
        exit(1);
    }
    // Read the total number of vertex in the graph
    int V;
    fscanf(file, "%d", &V);
    // Create the graph with the number of vertex read
    Graph *graph = createGraph(V);

    // Populate the Adjacency List with the edges
    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) != EOF)
        addEdge(graph, src - 1, dest - 1); // Adjust for 0-based indexing in C
    // Return the graph created
    fclose(file);
    return graph;
}

// Recursive function to find articulation points using DFS
void APUtil(Graph *graph, int u, bool visited[], int disc[], int low[], int parent[], bool isArticulationPoint[], int *time)
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
        // Current adjacent vertex
        int v = pCrawl->dest;
        // If "v" is not visited, then it is a tree edge
        if (!visited[v])
        {
            children++;
            // Set "u" as parent of "v"
            parent[v] = u;
            // Recursive call for vertex v
            APUtil(graph, v, visited, disc, low, parent, isArticulationPoint, time);
            // Check if the subtree rooted at "v" has a connection back to one of u's ancestors
            low[u] = (low[u] < low[v]) ? low[u] : low[v];
            // (1) "u" is an articulation point if it is the root and has two or more children
            if (parent[u] == -1 && children > 1)
            {
                isArticulationPoint[u] = true;
            }
            // (2) "u" is an articulation point if it is not the root and low value(v) >= discovery time(u)
            if (parent[u] != -1 && low[v] >= disc[u])
            {
                isArticulationPoint[u] = true;
            }
        }
        else if (v != parent[u])
        {
            // Update low[u] to the minimum of low[u] and disc[v] for back(<-) edge v-u
            low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
        }
        pCrawl = pCrawl->next; // Move to the next adjacent vertex
    }
}

// Find and print all articulation points
int findArticulationPoints(Graph *graph, bool *isArticulationPoint)
{
    // Print header for articulation points
    printf("%s%s:: ARTICULATION POINTS ::%s\n", bold, purple, reset);
    // Allocate memory for auxiliary arrays
    bool *visited = (bool *)malloc(graph->V * sizeof(bool));
    int *disc = (int *)malloc(graph->V * sizeof(int));
    int *low = (int *)malloc(graph->V * sizeof(int));
    int *parent = (int *)malloc(graph->V * sizeof(int));
    // Initialize time
    int time = 0;
    // Initialize number of articulation points found
    int count = 0;
    // Initialization of arrays
    for (int i = 0; i < graph->V; i++)
    {
        parent[i] = -1;                 // Initialize parent array
        visited[i] = false;             // Mark all vertices as not visited
        isArticulationPoint[i] = false; // Vertex at position "i" is not an articulation point
    }
    // For each vertex, if not visited, call DFS to find articulation points
    for (int i = 0; i < graph->V; i++)
    {
        if (!visited[i])
        {
            APUtil(graph, i, visited, disc, low, parent, isArticulationPoint, &time);
        }
    }
    // Print all articulation points found
    printf("%s%sArticulation points in the graph:%s ", bold, white, reset);
    for (int i = 0; i < graph->V; i++)
    {
        if (isArticulationPoint[i] == true)
        {
            printf("%s%s%d%s ", bold, white, i + 1, reset); // Adjusted for 1-based indexing
            // Increment number of articulation points found
            count++;
        }
    }
    // Freeing allocated memory
    free(visited);
    free(disc);
    free(low);
    free(parent);
    // Return the number of articulation points found in the graph
    return count;
}

// BFS to find the vertex and edges
void BFSBlock(Graph *graph, int startVertex, bool *visited, bool *isArticulationPoint, int *blockSize, bool *blockMembers)
{
    // Initialize a queue for BFS
    int *queue = (int *)malloc(graph->V * sizeof(int));
    int front = 0;
    int rear = 0;

    // Start with the vertex "startVertex"
    queue[rear++] = startVertex;
    // Mark vertex "startVertex" as visited
    visited[startVertex] = true; 
    // Mark vertex "startVertex" as part of this block
    blockMembers[startVertex] = true; 
    // Added vertex "startVertex" to the block
    (*blockSize)++; 
    // While has not reach rear of the queue
    while (front < rear)
    {
        // Get the first vertex in queue
        int current = queue[front++];
        // For all it's neighbours...
        AdjListNode *pCrawl = graph->array[current].head;
        while (pCrawl != NULL)
        {
            // Get the neighbour of the vertex
            int neighbour = pCrawl->dest;
            if (!visited[neighbour])
            {
                // Mark neighbour as visited
                visited[neighbour] = true;
                // Mark neighbour as part of this block
                blockMembers[neighbour] = true; 
                // If neighbour it's not an articulation point, continue exploring
                if (!isArticulationPoint[neighbour])
                    queue[rear++] = neighbour;
                // Add neighbour to the block
                (*blockSize)++;
            }
            // Get the next vertex
            pCrawl = pCrawl->next;
        }
    }
    // Free the allocated queue
    free(queue);
}

// Find and analyse the blocks of the graph
void analyzeBlocks(Graph *graph)
{
    // Array to track vertexs visited
    bool *visited = (bool *)malloc(graph->V * sizeof(bool));
    // Array to track articulation points
    bool *isArticulationPoint = (bool *)malloc(graph->V * sizeof(bool));
    // Array to track block members
    bool *blockMembers = (bool *)malloc(graph->V * sizeof(bool)); 
    // Initialize arrays
    for (int i = 0; i < graph->V; i++) 
    {
        visited[i] = false;
        blockMembers[i] = false;
    }
    // Find all articulation points in the graph
    int numArticulationPoints = findArticulationPoints(graph, isArticulationPoint);
    // Print header for block analysis
    printf("\n\n%s%s:: BLOCKS ANALYSIS ::%s\n", bold, purple, reset);
    // Initialize number of blocks found
    int numBlocks = 0;
    // Iterate over all vertex in the graph
    for (int i = 0; i < graph->V; i++)
    {
        // If the vertex is not visited or not an articulation point, start a new block
        if (!visited[i] || isArticulationPoint[i])
        {
            // Initialize size of the block
            int blockSize = 0; 
            // Initialize number of edges for the current block
            int numEdges = 0;
            // Initialize blockMembers array for the current block
            for (int j = 0; j < graph->V; j++)
                blockMembers[j] = false;
            // BFS to find the block size and mark block members
            BFSBlock(graph, i, visited, isArticulationPoint, &blockSize, blockMembers);
            // For all vertex in th graph
            for (int v = 0; v < graph->V; v++)
            {
                // If the vertex "v" belongs to the current block
                if (blockMembers[v])
                {
                    AdjListNode *pCrawl = graph->array[v].head;
                    // Iterate for each neighbor of "v"
                    while (pCrawl != NULL)
                    {
                        // If the neighbor of "v" is in the block
                        if (blockMembers[pCrawl->dest])
                            numEdges++; // Add the edge to the block
                        // Get the next neighbour
                        pCrawl = pCrawl->next;
                    }
                }
            }
            // If it's a valid block(has at least 1 vertex)
            if (blockSize > 0)
            {
                // Since every edge is counted twice(v-u, u-v) in an undirected graph, divide by 2
                printf("%sBlock %d: Size %d, Edges: %d%s\n\n", bold, numBlocks + 1, blockSize, numEdges / 2, reset);
                // Increase the number of blocks found
                numBlocks++;
            }
        }
    }
    // Summarize the blocks info
    printf("%s%sTotal number of blocks: %d%s\n", bold, purple, numBlocks, reset);
    // Free allocated memory
    free(visited);
    free(isArticulationPoint);
    free(blockMembers);
    freeGraph(graph);
    return;
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
    Graph *graph = readFile(filename);
    if (graph != NULL)
    {
        printGraph(graph);
        analyzeBlocks(graph);       
        return 0;
    }
    printf("%s%sERROR => Could not initialize graph from file. Exiting...%s\n", bold, red, reset);
    return 1;
}
