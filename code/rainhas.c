#include "rainhas.h"
// #include "lista.h"
#include <stdlib.h>
#include <stdio.h>

// Função para verificar se é seguro colocar uma rainha em uma determinada posição 
int is_safe(unsigned int* board, unsigned int row, unsigned int col, casa* c, unsigned int k) {
    for (unsigned int p = 0; p < k; ++p) {
        if (c[p].linha - 1 == (unsigned int)row && c[p].coluna - 1 == (unsigned int)col) {
            return 0;
        }
    }
    for (unsigned int i = 0; i < row; i++) {
        if (board[i] == col || board[i] == (col - (row - i)) || board[i] == (col + (row - i))) {
            return 0;
        }
    }
    return 1;
}

// Função auxiliar para resolver o problema das N-rainhas com backtracking
int solve_n_queens_bt(unsigned int* board, unsigned int row, unsigned int n, casa* c, unsigned int k) {
    if (row >= n) {
        return 1;
    }
    for (unsigned int i = 0; i < n; i++) {
        if (is_safe(board, row, i, c, k)) {
            board[row] = i;
            if (solve_n_queens_bt(board, row + 1, n, c, k)) {
                return 1;
            }
            board[row] = 0;  // Backtrack
        }
    }
    return 0;
}

unsigned int* rainhas_bt(unsigned int n, unsigned int k, casa* c, unsigned int* r) {
    // Verificar se todas as casas estão proibidas
    if (k >= n * n) {
        for (unsigned int i = 0; i < n; i++) {
            r[i] = 0;
        }
        return r;
    }

    unsigned int* board = (unsigned int*)malloc(n * sizeof(unsigned int));
    for (unsigned int i = 0; i < n; i++) {
        board[i] = 0;
        r[i] = 0;
    }

    if (solve_n_queens_bt(board, (unsigned int)0, n, c, k)) {
        for (unsigned int i = 0; i < n; i++) {
            r[i] = board[i] + 1;  // Adiciona 1 ao índice da coluna
        }
    }

    free(board);
    return r;
}

// Função para resolver o problema das N-rainhas com conjuntos independentes
int solve_n_queens_ci(unsigned int *board, unsigned int row, unsigned int n, casa *c, unsigned int k)
{
    if (row >= n)
    {
        return 1;
    }
    for (unsigned int i = 0; i < n; i++)
    {
        int forbidden = 0;
        for (unsigned int p = 0; p < k; ++p)
        {
            if (c[p].linha - 1 == row && c[p].coluna - 1 == i)
            {
                forbidden = 1;
                break;
            }
        }
        if (forbidden)
            continue;

        int conflict = 0;
        for (unsigned int j = 0; j < row; j++)
        {
            if (board[j] == i || abs((int)board[j] - (int)i) == (row - j))
            {
                conflict = 1;
                break;
            }
        }
        if (!conflict)
        {
            board[row] = i;
            if (solve_n_queens_ci(board, row + 1, n, c, k))
            {
                return 1;
            }
            board[row] = 0; // Backtrack
        }
    }
    return 0;
}

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r)
{
    // Verificar se todas as casas estão proibidas
    if (k >= n * n)
    {
        for (unsigned int i = 0; i < n; i++)
        {
            r[i] = 0;
        }
        return r;
    }

    unsigned int *board = (unsigned int *)malloc(n * sizeof(unsigned int));
    for (unsigned int i = 0; i < n; i++)
    {
        board[i] = 0;
        r[i] = 0;
    }

	unsigned int graph_size = n * n;
	t_lista *graph = (t_lista *)malloc(graph_size * sizeof(t_lista));
	create_graph(board, graph, graph_size, n);

	for(unsigned int i = 0; i < graph_size; i++) {
        printf("Lista %d: ", i);
		imprime_lista(&graph[i]);
	}

    if (solve_n_queens_ci(board, (unsigned int)0, n, c, k))
    {
        for (unsigned int i = 0; i < n; i++)
        {
            r[i] = board[i] + 1; // Adiciona 1 ao índice da coluna
        }
    }

    free(board);
    return r;
}

void create_graph(unsigned int *board, t_lista* graph, unsigned int graph_size, unsigned int n)
{
	for (unsigned int i = 0; i < graph_size; i++)
	{
		cria_lista(&graph[i]);
	}

	// itera pelo tabuleiro colocando uma rainha em cada posicao para ver quais seriam atacadas por ela
	for (unsigned int row = 0; row < n; row++)
	{
		for (unsigned int col = 0; col < n; col++)
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
						insere_fim_lista(i*n+j, &graph[row * n + col]);
					}
				}
			}
		}

		board[row] = 0;
	}
}

// IMPLEMENTACOES DA LISTA
void imprime_lista(t_lista *l) {
	Node *aux;

	if (lista_vazia (l))
		return;
	aux = l->ini;
	while (aux != NULL) {
		printf("%d ", aux->v);
		aux = aux->next;
	}
	printf("\n");
}

int cria_lista(t_lista *l) {
	l->ini = NULL;
	l->tamanho = 0;
	return 1;
}

int insere_fim_lista(unsigned int x, t_lista *l) {
	Node *new, *aux;

	if(lista_vazia(l))
		return insere_inicio_lista (x, l);

	/* Caso a lista não esteja vazia, ela acha o fim dela */
	new = (Node *) malloc (sizeof(Node));
	if(new == NULL) {
		printf("Memória insuficiente\n");
		return 0;
	}

	new->v = x;
	new->next = NULL;
	aux = l->ini;
	while (aux->next != NULL)
		aux = aux->next;
	l->tamanho++;
	aux->next = new;
	return 1;
}

int lista_vazia(t_lista *l) {
	if(l->ini == NULL)
		return 1;
	return 0;
}

int insere_inicio_lista(unsigned int x, t_lista *l) {
	Node *new;

	new = (Node *) malloc(sizeof (Node));
	if(new == NULL) {
		printf("Memória insuficiente\n");
		return 0;
	}

	new->v = x;
	new->next = NULL;
	l->tamanho++;
	/* Caso a lista esteja vazia */
	if(lista_vazia (l)) {
		l->ini = new;
		return 1;
	}
	new->next = l->ini;
	l->ini = new;
	return 1;
}