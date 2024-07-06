#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>

// Função para verificar se é seguro colocar uma rainha em uma determinada posição
int is_safe(unsigned int *board, unsigned int row, unsigned int col, casa *c, unsigned int k)
{
    for(unsigned int p = 0; p < k; ++p)
    {
        if(c[p].linha - 1 == (unsigned int)row && c[p].coluna - 1 == (unsigned int)col)
        {
            return 0;
        }
    }
    for(unsigned int i = 0; i < row; i++)
    {
        if(board[i] == col || board[i] == (col -(row - i)) || board[i] == (col +(row - i)))
        {
            return 0;
        }
    }
    return 1;
}

// Função auxiliar para resolver o problema das N-rainhas com backtracking
int solve_n_queens_bt(unsigned int *board, unsigned int row, unsigned int n, casa *c, unsigned int k)
{
    if(row >= n)
    {
        return 1;
    }
    for(unsigned int i = 0; i < n; i++)
    {
        if(is_safe(board, row, i, c, k))
        {
            board[row] = i;
            if(solve_n_queens_bt(board, row + 1, n, c, k))
            {
                return 1;
            }
            board[row] = 0; // Backtrack
        }
    }
    return 0;
}

unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r)
{
    // Verificar se todas as casas estão proibidas
    if(k >= n * n)
    {
        for(unsigned int i = 0; i < n; i++)
        {
            r[i] = 0;
        }
        return r;
    }

    unsigned int *board = (unsigned int *)malloc(n * sizeof(unsigned int));
    for(unsigned int i = 0; i < n; i++)
    {
        board[i] = 0;
        r[i] = 0;
    }

    if(solve_n_queens_bt(board,(unsigned int)0, n, c, k))
    {
        for(unsigned int i = 0; i < n; i++)
        {
            r[i] = board[i] + 1; // Adiciona 1 ao índice da coluna
        }
    }

    free(board);
    return r;
}

// Função para resolver o problema das N-rainhas com conjuntos independentes
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r)
{
    // Verificar se todas as casas estão proibidas
    if(k >= n * n)
    {
        for(unsigned int i = 0; i < n; i++)
        {
            r[i] = 0;
        }
        return r;
    }

    unsigned int *board = (unsigned int *)malloc(n * sizeof(unsigned int));
    for(unsigned int i = 0; i < n; i++)
    {
        board[i] = 0;
        r[i] = 0;
    }

    unsigned int graph_size = n * n;
    t_lista *graph = (t_lista *)malloc(graph_size * sizeof(t_lista));
    create_graph(board, graph, graph_size, n);

    // criar conjunto independente
    t_lista independet_set, available_vertices;
    cria_lista(&independet_set);

    // criar conjunto available_vertices - conjunto que contém vértices não proibidos do grafo
    create_available_vertices(graph_size, c, k, &available_vertices, n);

    t_lista *result = ConjIndep(graph, n, &independet_set, &available_vertices);
    if(result)
    {
        Node *aux = result->ini;
        unsigned int i = 0;
        while(aux != NULL) {
            unsigned int coluna = aux->v % n;
            printf("%d\n", coluna);
            r[i] = coluna+1;
            aux = aux->next;
            i++;
        }
    }

    free(board);
    libera_listas(graph, graph_size, available_vertices, independet_set);

    return r;
}

void create_graph(unsigned int *board, t_lista *graph, unsigned int graph_size, unsigned int n)
{
    for(unsigned int i = 0; i < graph_size; i++)
    {
        cria_lista(&graph[i]);
    }

    // itera pelo tabuleiro colocando uma rainha em cada posicao para ver quais seriam atacadas por ela
    for(unsigned int row = 0; row < n; row++)
    {
        for(unsigned int col = 0; col < n; col++)
        {
            board[row] = col;

            for(unsigned int i = 0; i < n; i++)
            {
                for(unsigned int j = 0; j < n; j++)
                {
                    if(i == row && j == col)
                        continue;

                    if((row == i || j == col || abs((int)(row - i)) == abs((int)(j - col))))
                    {
                        insere_fim_lista(i * n + j, &graph[row * n + col]);
                    }
                }
            }
        }

        board[row] = 0;
    }
}

void create_available_vertices(unsigned int graph_size, casa *c, unsigned int k, t_lista *available_vertices, unsigned int n)
{
    cria_lista(available_vertices);

    for(unsigned int graph_vertice = 0; graph_vertice < graph_size; graph_vertice++)
    {
        int node_blocked = 0;
        unsigned int cell_blocked = 0;
        while(cell_blocked < k && !node_blocked)
        {
            unsigned int line = c[cell_blocked].linha-1;
            unsigned int column = c[cell_blocked].coluna-1;

            if(line * n + column == graph_vertice) node_blocked = 1;

            cell_blocked++;
        }

        if(!node_blocked)
        {
            insere_fim_lista(graph_vertice, available_vertices);
        }
    }
}

void libera_listas(t_lista *graph, unsigned int graph_size, t_lista available_vertices, t_lista independet_set)
{
    for(unsigned int i = 0; i < graph_size; i++)
    {
        destroi_lista(&graph[i]);
    }
    free(graph);

    destroi_lista(&available_vertices);
    destroi_lista(&independet_set);
}

// IMPLEMENTACOES DA LISTA
void imprime_lista(t_lista *l)
{
    Node *aux;

    if(lista_vazia(l))
        return;

    aux = l->ini;
    while(aux != NULL)
    {
        printf("%d ", aux->v);
        aux = aux->next;
    }
    printf("\n");
}

int cria_lista(t_lista *l)
{
    l->ini = NULL;
    l->tamanho = 0;
    return 1;
}

int insere_fim_lista(unsigned int x, t_lista *l)
{
    Node *new, *aux;

    if(lista_vazia(l))
        return insere_inicio_lista(x, l);

    /* Caso a lista não esteja vazia, ela acha o fim dela */
    new = (Node *)malloc(sizeof(Node));
    if(new == NULL)
    {
        printf("Memória insuficiente\n");
        return 0;
    }

    new->v = x;
    new->next = NULL;
    aux = l->ini;

    while(aux->next != NULL)
        aux = aux->next;
    l->tamanho++;
    aux->next = new;
    return 1;
}

int lista_vazia(t_lista *l)
{
    if(l->ini == NULL)
        return 1;
    return 0;
}

int insere_inicio_lista(unsigned int x, t_lista *l)
{
    Node *new;

    new = (Node *)malloc(sizeof(Node));
    if(new == NULL)
    {
        printf("Memória insuficiente\n");
        return 0;
    }

    new->v = x;
    new->next = NULL;
    l->tamanho++;

    /* Caso a lista esteja vazia */
    if(lista_vazia(l))
    {
        l->ini = new;
        return 1;
    }
    new->next = l->ini;
    l->ini = new;
    return 1;
}

unsigned int remove_primeiro_lista(unsigned int *item, t_lista *l) {
	Node *aux;

	if(lista_vazia(l))
		return 0;

	/* Caso a lista não esteja vazia */
	l->tamanho--;
	*item = l->ini->v;
	aux = l->ini;
	l->ini = l->ini->next;
	free(aux);
	return 1;
}

unsigned int remove_item_lista(unsigned int chave, unsigned int *item, t_lista *l) {
	Node *aux, *anterior;

	if(lista_vazia(l))
		return 0;

	/* Caso o elemento esteja na primeira posição */
	if(l->ini->v == chave) {
		return remove_primeiro_lista(item, l);
	}

	aux = l->ini->next;
	anterior = l->ini;
	while((aux->next != NULL) && (aux->v != chave)) {
		anterior = aux;
		aux = aux->next;
	}
	/* Caso tenha chegado ao fim da lista e o elemento não foi achado */
	if((aux->next == NULL) && (aux->v != chave)) {
		printf("Nao removido: elemento nao encontrado.\n");
		return 0;
	}
	/* Caso o elemento esteja na ultima posição */
	else if((aux->next == NULL) && (aux->v == chave)) {
		*item = aux->v;
		anterior->next = NULL;
	}
	else {
		*item = aux->v;
		anterior->next = aux->next;
	}
	l->tamanho--;
	free(aux);
	return 1;
}

int pertence_lista (unsigned int chave, t_lista *l) {
	Node *aux;

	if (lista_vazia (l))
		return 0;

	aux = l->ini;
	while ((aux->next != NULL) && (aux->v != chave))
		aux = aux->next;
	if (aux->v == chave)
		return 1;
	return 0;
}

void destroi_lista(t_lista *l) {

	if(lista_vazia(l)) {
		printf("Nao destruida, lista vazia\n");
		return;
	}

	Node *aux = l->ini;
	Node *aux_2 = aux->next;

	while(aux_2 != NULL) {
		free(aux);
		aux = aux_2;
		aux_2 = aux_2->next;
	}
	free(aux);
	l->tamanho = 0;
	l->ini = NULL;
}

t_lista* ConjIndep(t_lista *graph, unsigned int n, t_lista *independent_set, t_lista *available_vertices) {
    if(independent_set->tamanho == n) {
        return independent_set;
    }

    if(independent_set->tamanho + available_vertices->tamanho < n) {
        return NULL;
    }

    unsigned int v;
    if(remove_primeiro_lista(&v, available_vertices) == 0) {
        return NULL;
    }

    insere_fim_lista(v, independent_set);

    // remove os vizinhos de v de available_vertices
    Node *aux = graph[v].ini;
    while(aux != NULL) {
        unsigned int node_to_remove = aux->v;
        unsigned int item;

        if(pertence_lista(node_to_remove, available_vertices)) {
            remove_item_lista(node_to_remove, &item, available_vertices);
        }

        aux = aux->next;
    }

    t_lista *result = ConjIndep(graph, n, independent_set, available_vertices);

    if(result) {
        return result;
    }

    // retornar o estado anterior
    // remove_item_lista(v, &v, independent_set);
    // aux = graph[v].ini;
    // while(aux != NULL) {
    //     unsigned int node_to_add = aux->v;

    //     if(!pertence_lista(node_to_add, available_vertices)) {
    //         insere_inicio_lista(node_to_add, available_vertices);
    //     }

    //     aux = aux->next;
    // }

    return ConjIndep(graph, n, independent_set, available_vertices);
}