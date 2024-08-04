#include <stdio.h>
#include <stdlib.h>
#include "findBlocks.h"

// ANSI escape codes for colors and styles
const char *reset = "\033[0m";   // Reset all styles
const char *bold = "\033[1m";    // Bold text
const char *red = "\033[31m";    // Red
const char *white = "\033[37m";  // White
const char *purple = "\033[95m"; // Bright magenta (light purple)

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
    // Each vertex will have its own adjacency list
    graph->array = (AdjList *)malloc((size_t)V * sizeof(AdjList));
    // Loop over all vertices
    for (int i = 0; i < V; ++i)
        // Initialize each adjacency list as empty by setting its head to NULL
        graph->array[i].head = NULL;
    // Return the newly created graph structure
    return graph;
}

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

Graph *readFile(const char *filename)
{
    // Try to open the file read-only
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // Could not open the file
        printf("%s%sERROR => Could not open file \"%s\"\n%s", bold, red, filename, reset);
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
    fclose(file);
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
        // Current adjacent vertex
        int v = pCrawl->dest;
        // If "v" is not visited, then it is a tree edge
        if (!visited[v])
        {
            children++;
            // Set "u" as parent of "v"
            parent[v] = u;
            // Recursive call for vertex v
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
            low[u] = (low[u] < disc[v]) ? low[u] : disc[v]; // Update low(u) to the minimum of low(u) and disc(v) for back(<-) edge v-u
        pCrawl = pCrawl->next;                              // Move to the next adjacent vertex
    }
}

void findAllArticulationPoints(Graph *graph, bool *isArticulationPoint)
{
    // Print header for articulation points
    printf("%s%s:: ARTICULATION POINTS ::%s\n", bold, purple, reset);
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
        isArticulationPoint[i] = false; // Vertex at position "i" is not an articulation point
    }
    // For each vertex, if not visited, call DFS to find articulation points
    for (int i = 0; i < graph->V; i++)
        if (!visited[i])
            DFSarticulationPoints(graph, i, visited, disc, low, parent, isArticulationPoint, &time);
    // Print all articulation points found
    printf("%s%sArticulation points in the graph:%s ", bold, white, reset);
    for (int i = 0; i < graph->V; i++)
        if (isArticulationPoint[i] == true)
            printf("%s%s%d%s ", bold, white, i + 1, reset); // Adjusted for 1-based indexing
    // Freeing allocated memory
    free(visited);
    free(disc);
    free(low);
    free(parent);
}

void BFSblocks(Graph *graph, int startVertex, bool *visited, bool *isArticulationPoint, int *blockSize, bool *blockMembers)
{
    // Initialize a queue for BFS
    int *queue = (int *)malloc((size_t)graph->V * sizeof(int));
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
    // Print the vertex added to the block
    printf("Vertex %d added to the block\n", startVertex + 1); // +1 for 1-based indexing
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
                // Print the vertex added to the block
                printf("Vertex %d added to the block\n", neighbour + 1); // +1 for 1-based indexing 
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

void findAllBlocks(Graph *graph)
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
    // Print header for block analysis
    printf("\n\n%s%s:: BLOCKS ANALYSIS ::%s\n", bold, purple, reset);
    // Initialize number of blocks found
    int numBlocks = 0;
    // Iterate over all vertices in the graph
    for (int i = 0; i < graph->V; i++)
    {
        // If the vertex is not visited or is an articulation point, start a new block
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
            BFSblocks(graph, i, visited, isArticulationPoint, &blockSize, blockMembers);
            // For all vertices in the graph
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
                        // Get the next neighbor
                        pCrawl = pCrawl->next;
                    }
                }
            }
            // If it's a valid block (has at least 1 vertex)
            if (blockSize > 0)
            {
                // Since every edge is counted twice (v-u, u-v) in an undirected graph, divide by 2
                numEdges /= 2;
                // Store the block information
                blocksInfo[numBlocks].vertexes = (unsigned int)blockSize;
                blocksInfo[numBlocks].edges = (unsigned int)numEdges;
                // Print the block information found
                printf("%sFound Block %d: Number of Vertexes %d, Number of Edges: %d%s\n\n", bold, numBlocks + 1, blockSize, numEdges, reset);
                // Increase the number of blocks found
                numBlocks++;
            }
        }
    }
    // Print all blocks information
    printBlocksInfo(blocksInfo, numBlocks);
    // Free allocated memory
    free(visited);
    free(isArticulationPoint);
    free(blockMembers);
    free(blocksInfo);
    freeGraph(graph);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("%sUsage: %s <path/to/input_filename>%s\n", bold, argv[0], reset);
        return 1;
    }
    // Name of the file where the graph is located
    const char *filename = argv[1];
    // Read graph from file specified
    Graph *graph = readFile(filename);
    if (graph != NULL)
    {
        printGraph(graph);
        printf("\n");
        findAllBlocks(graph);
        return 0;
    }
    printf("%s%sERROR => Could not create Graph from file \"%s\".%s\n", bold, red, filename, reset);
    return 1;
}
