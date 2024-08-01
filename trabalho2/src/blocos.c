#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "blocos.h"

// Definições para representar o grafo
typedef struct AdjListNode
{
    int dest;
    struct AdjListNode *next;
} AdjListNode;

typedef struct AdjList
{
    AdjListNode *head;
} AdjList;

typedef struct Graph
{
    int V;
    AdjList *array;
} Graph;

// Funções auxiliares para o grafo
AdjListNode *newAdjListNode(int dest)
{
    AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int V)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList *)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

void addEdge(Graph *graph, int src, int dest)
{
    AdjListNode *newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Função para encontrar os blocos
void DFS(Graph *graph, int u, bool *visited, int *disc, int *low, int *parent, bool *articulation_point, int *time, int *num_blocos, info_t *blocos, int *edge_count)
{
    static int time = 0;
    int children = 0;
    visited[u] = true;
    disc[u] = low[u] = ++(*time);

    for (AdjListNode *v = graph->array[u].head; v != NULL; v = v->next)
    {
        int v_id = v->dest;
        if (!visited[v_id])
        {
            children++;
            parent[v_id] = u;
            DFS(graph, v_id, visited, disc, low, parent, articulation_point, time, num_blocos, blocos, edge_count);
            low[u] = (low[u] < low[v_id]) ? low[u] : low[v_id];

            if (parent[u] == -1 && children > 1)
                articulation_point[u] = true;

            if (parent[u] != -1 && low[v_id] >= disc[u])
                articulation_point[u] = true;

            if (low[v_id] > disc[u])
            {
                // É uma ponte
                (*num_blocos)++;
                blocos[*num_blocos].vertices = children + 1; // aproximado
                blocos[*num_blocos].arestas = (*edge_count) / 2;
                *edge_count = 0;
            }
        }
        else if (v_id != parent[u])
            low[u] = (low[u] < disc[v_id]) ? low[u] : disc[v_id];
    }
}

info_t *info_blocos(unsigned int *num_blocos)
{
    int V, src, dest;
    scanf("%d", &V);

    Graph *graph = createGraph(V);
    while (scanf("%d %d", &src, &dest) != EOF)
    {
        addEdge(graph, src - 1, dest - 1);
    }

    bool *visited = (bool *)malloc(V * sizeof(bool));
    int *disc = (int *)malloc(V * sizeof(int));
    int *low = (int *)malloc(V * sizeof(int));
    int *parent = (int *)malloc(V * sizeof(int));
    bool *articulation_point = (bool *)malloc(V * sizeof(bool));
    int *edge_count = (int *)malloc(sizeof(int));
    *edge_count = 0;

    for (int i = 0; i < V; i++)
    {
        visited[i] = false;
        parent[i] = -1;
        articulation_point[i] = false;
    }

    info_t *blocos = (info_t *)malloc(V * sizeof(info_t));
    *num_blocos = 0;

    for (int i = 0; i < V; i++)
    {
        if (visited[i] == false)
        {
            DFS(graph, i, visited, disc, low, parent, articulation_point, time, num_blocos, blocos, edge_count);
        }
    }

    // Limpeza de memória alocada
    free(visited);
    free(disc);
    free(low);
    free(parent);
    free(articulation_point);
    free(edge_count);

    return blocos;
}
