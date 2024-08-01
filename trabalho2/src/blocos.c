#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "blocos.h"

// Definições para representar o grafo
typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

typedef struct AdjList {
    AdjListNode* head;
} AdjList;

typedef struct Graph {
    int V;
    AdjList* array;
} Graph;

// Prototipos das funções
AdjListNode* newAdjListNode(int dest);
Graph* createGraph(int V);
void addEdge(Graph* graph, int src, int dest);
void DFS(Graph* graph, int u, bool* visited, int* disc, int* low, int* parent, bool* articulation_point, int* current_vertices, int* current_edges);

AdjListNode* newAdjListNode(int dest) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList*)malloc((size_t)V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

void DFS(Graph* graph, int u, bool* visited, int* disc, int* low, int* parent, bool* articulation_point, int* current_vertices, int* current_edges) {
    static int time = 0;
    int children = 0; // Adicionando a variável children
    visited[u] = true;
    disc[u] = low[u] = ++time;
    (*current_vertices)++;

    for (AdjListNode* v = graph->array[u].head; v != NULL; v = v->next) {
        int v_id = v->dest;
        if (!visited[v_id]) {
            children++;
            (*current_edges)++;
            parent[v_id] = u;
            DFS(graph, v_id, visited, disc, low, parent, articulation_point, current_vertices, current_edges);
            low[u] = (low[u] < low[v_id]) ? low[u] : low[v_id];

            if ((parent[u] == -1 && children > 1) || (parent[u] != -1 && low[v_id] >= disc[u])) {
                articulation_point[u] = true;
            }
        } else if (v_id != parent[u]) {
            low[u] = (low[u] < disc[v_id]) ? low[u] : disc[v_id];
        }
    }
}

info_t* info_blocos(unsigned int* num_blocos) {
    int V, src, dest;
    if (scanf("%d", &V) != 1) {
        printf("Erro ao ler o número de vértices.\n");
        return NULL;
    }

    Graph* graph = createGraph(V);
    if (!graph) {
        printf("Erro ao criar o grafo.\n");
        return NULL;
    }

    while (scanf("%d %d", &src, &dest) == 2) {
        if (src < 1 || src > V || dest < 1 || dest > V) {
            printf("Aresta inválida: %d %d\n", src, dest);
            continue;
        }
        addEdge(graph, src - 1, dest - 1);
    }

    bool* visited = (bool*)malloc((size_t)V * sizeof(bool));
    int* disc = (int*)malloc((size_t)V * sizeof(int));
    int* low = (int*)malloc((size_t)V * sizeof(int));
    int* parent = (int*)malloc((size_t)V * sizeof(int));
    bool* articulation_point = (bool*)malloc((size_t)V * sizeof(bool));

    for (int i = 0; i < V; i++) {
        visited[i] = false;
        parent[i] = -1;
        articulation_point[i] = false;
    }

    info_t* blocos = (info_t*)malloc((size_t)V * sizeof(info_t));
    *num_blocos = 0;

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            int current_vertices = 0;
            int current_edges = 0;
            DFS(graph, i, visited, disc, low, parent, articulation_point, &current_vertices, &current_edges);
            if (current_vertices > 0) {
                blocos[*num_blocos].vertices = (unsigned int)current_vertices;
                blocos[*num_blocos].arestas = (unsigned int)current_edges;
                (*num_blocos)++;
            }
        }
    }

    // Limpeza de memória alocada
    free(visited);
    free(disc);
    free(low);
    free(parent);
    free(articulation_point);

    return blocos;
}
