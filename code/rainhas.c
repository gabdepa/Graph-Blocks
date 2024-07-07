#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>

/**************************************************ESTRUTURAS**************************************************/
// Estrutura para uma casa do tabuleiro
typedef struct casa
{
  unsigned int linha, coluna;
} casa;

// Estrutura para nós do grafo
struct Node {
    unsigned int v;
    struct Node* next;
};
typedef struct Node Node;

// Estrutura para lista
struct t_lista {
    Node *ini;
    unsigned int tamanho;
};
typedef struct t_lista t_lista;
/****************************************************************************************************/

/**************************************************ASSINATURAS**************************************************/
//Lista
void imprime_lista (t_lista *l);
int cria_lista(t_lista *l);
int insere_fim_lista(unsigned int x, t_lista *l);
int lista_vazia(t_lista *l);
int insere_inicio_lista(unsigned int x, t_lista *l);
unsigned int remove_primeiro_lista (unsigned int *item, t_lista *l);
unsigned int remove_item_lista (unsigned int chave, unsigned int *item, t_lista *l);
void destroi_lista (t_lista *l);
void libera_listas(t_lista *graph, unsigned int graph_size, t_lista available_vertices, t_lista independet_set);
int pertence_lista (unsigned int chave, t_lista *l);
int insere_ordenado_lista (unsigned int x, t_lista *l);
int copia_lista (t_lista *lista1, t_lista *lista2);

// Protótipos
int is_safe(unsigned int* board, unsigned int row, unsigned int col, casa* c, unsigned int k);
int solve_n_queens_bt(unsigned int *board, unsigned int row, unsigned int n, casa *c, unsigned int k);
int solve_n_queens_ci(unsigned int* board, unsigned int row, unsigned int n, casa* c, unsigned int k);
void create_graph(unsigned int *board, t_lista* graph, unsigned int graph_size, unsigned int n);
void create_available_vertices(unsigned int graph_size, casa *c, unsigned int k, t_lista *available_vertices, unsigned int n);
t_lista* ConjIndep(t_lista *graph, unsigned int n, t_lista *independent_set, t_lista *available_vertices);

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n rainhas
// com casas proibidas usando backtracking
//    n é o tamanho (número de linhas/colunas) do tabuleiro
//    c é um vetor de k 'struct casa' indicando as casas proibidas
//    r é um vetor de n posições (já alocado) a ser preenchido com a resposta:
//      r[i] = j > 0 indica que a rainha da linha i+1 fica na coluna j;
//      r[i] = 0     indica que não há rainha nenhuma na linha i+1
// devolve r
unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r);

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n rainhas
// com casas proibidas usando backtracking
// n, c, r e o valor de retorno são como em rainhas_bt
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r);
/****************************************************************************************************/


/**************************************************BackTracking***************************************************/
// Função para verificar se é seguro colocar uma rainha em uma determinada posição
int is_safe(unsigned int *board, unsigned int row, unsigned int col, casa *c, unsigned int k)
{
    for(unsigned int p = 0; p < k; ++p)
    {
        if(c[p].linha - 1 == row && c[p].coluna - 1 == col)
        {
            return 0;
        }
    }
    for(unsigned int i = 0; i < row; i++)
    {
        // como armazenamos o col+1 no tabuleiro, somamos 1 para comparar
        if(board[i] == col+1 || board[i] == (col+1 -(row - i)) || board[i] == (col+1 +(row - i)))
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
            // 0 apenas se nao tiver rainha
            board[row] = i+1;
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
            r[i] = board[i]; // Adiciona 1 ao índice da coluna
        }
    }

    free(board);
    return r;
}
/**************************************************BackTracking***************************************************/

/**************************************************Conjuntos Independentes***************************************************/
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

    // cria conjunto independente - conjunto solucao do problema
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
            // retorna a coluna em que esta de acordo com o valor do vertice
            // -1 pois armazenamos a col+1 no tabuleiro e se for colocado na ultima coluna
            // o valor do vértice % n seria 0, e em seguida adicionamos 1 novamente ao valor da coluna
            unsigned int coluna = (aux->v-1) % n;
            r[i] = coluna+1;
            aux = aux->next;
            i++;
        }
    }

    free(board);
    libera_listas(graph, graph_size, available_vertices, independet_set);

    return r;
}

/**************************************************Implementações de Grafo***************************************************/
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

                    // se a posicao do tabuleiro pode ser atacada, entao ela e um vertice adjacente
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

// identifica quais vertices do grafo nao estao bloqueados para montar
// o conjunto de vertices que podemos inserir rainhas
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


/**************************************************Implementações da Lista***************************************************/ 
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

    // Caso a lista não esteja vazia, ela acha o fim dela
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

    // Caso a lista esteja vazia
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

	// Caso a lista não esteja vazia
	l->tamanho--;
	*item = l->ini->v;
	aux = l->ini;
	l->ini = l->ini->next;
	free(aux);
	return 1;
}

unsigned int remove_item_lista(unsigned int v, unsigned int *item, t_lista *l) {
	Node *aux, *anterior;

	if(lista_vazia(l))
		return 0;

	// Caso o elemento esteja na primeira posição
	if(l->ini->v == v) {
		return remove_primeiro_lista(item, l);
	}

	aux = l->ini->next;
	anterior = l->ini;
	while((aux->next != NULL) && (aux->v != v)) {
		anterior = aux;
		aux = aux->next;
	}
	// Caso tenha chegado ao fim da lista e o elemento não foi achado
	if((aux->next == NULL) && (aux->v != v)) {
		printf("Nao removido: elemento nao encontrado.\n");
		return 0;
	}
	// Caso o elemento esteja na ultima posição
	else if((aux->next == NULL) && (aux->v == v)) {
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

int pertence_lista (unsigned int v, t_lista *l) {
	Node *aux;

	if (lista_vazia (l))
		return 0;

	aux = l->ini;
	while ((aux->next != NULL) && (aux->v != v))
		aux = aux->next;
	if (aux->v == v)
		return 1;
	return 0;
}

void destroi_lista(t_lista *l) {

	if(lista_vazia(l)) {
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

int copia_lista (t_lista *lista1, t_lista *lista2) {
	Node *aux_l;

	if (lista_vazia (lista1))
		return 0;

	aux_l = lista1->ini;
	insere_inicio_lista (aux_l->v, lista2);
	while (aux_l->next != NULL) {
		aux_l = aux_l->next;
		insere_fim_lista (aux_l->v, lista2);
	}
	lista2->tamanho = lista1->tamanho;
	return 1;
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

    // copia os vertices disponiveis caso precise voltar o estado
    t_lista *available_vertices_copy = (t_lista *)malloc(sizeof(t_lista));
    cria_lista(available_vertices_copy);
    copia_lista(available_vertices, available_vertices_copy);

    insere_fim_lista(v+1, independent_set);

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

    // retornar o estado anterior e chama a recursão sem o vértice v
    remove_item_lista(v+1, &v, independent_set);

    return ConjIndep(graph, n, independent_set, available_vertices_copy);
}
/*****************************************************************************************************/