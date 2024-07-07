#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>

/**************************************************BACKTRACKING***************************************************/
// Função para verificar se é seguro colocar uma rainha em uma determinada posição
int is_safe(unsigned int *board, unsigned int row, unsigned int col, casa *c, unsigned int k)
{
    // Verifica se a posição está em uma das casas proibidas
    for (unsigned int p = 0; p < k; ++p)
    {
        if (c[p].linha - 1 == row && c[p].coluna - 1 == col)
        {
            return 0; // Se a posição for proibida, não é seguro
        }
    }

    // Verifica se há conflitos com outras rainhas já colocadas
    for (unsigned int i = 0; i < row; i++)
    {
        // Como armazenamos o col+1 no tabuleiro, somamos 1 para comparar
        if (board[i] == col + 1 || board[i] == (col + 1 - (row - i)) || board[i] == (col + 1 + (row - i)))
        {
            return 0; // Se houver conflito, não é seguro
        }
    }

    // Se não houver conflitos, retorna 1 (é seguro)
    return 1;
}

// Função auxiliar para resolver o problema das N-rainhas com backtracking
int solve_n_queens_bt(unsigned int *board, unsigned int row, unsigned int n, casa *c, unsigned int k)
{
    if (row >= n)
    {
        return 1;
    }
    for (unsigned int i = 0; i < n; i++)
    {
        if (is_safe(board, row, i, c, k))
        {
            // 0 apenas se nao tiver rainha
            board[row] = i + 1;
            if (solve_n_queens_bt(board, row + 1, n, c, k))
            {
                return 1;
            }
            board[row] = 0; // Backtrack
        }
    }
    return 0;
}

// Função para resolver o problema das N-rainhas com Backtracking
unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r)
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

    if (solve_n_queens_bt(board, (unsigned int)0, n, c, k))
    {
        for (unsigned int i = 0; i < n; i++)
        {
            r[i] = board[i]; // Adiciona 1 ao índice da coluna
        }
    }

    free(board);
    return r;
}
/**************************************************BACKTRACKING***************************************************/

/**************************************************CONJUNTOS INDEPENDENTES***************************************************/
// Função para criar um grafo representando o problema das N-rainhas
void create_graph(unsigned int *board, t_lista *graph, unsigned int graph_size, unsigned int n)
{
    // Inicializa cada lista no grafo
    for (unsigned int i = 0; i < graph_size; i++)
    {
        cria_lista(&graph[i]);
    }

    // Itera pelo tabuleiro colocando uma rainha em cada posição para ver quais seriam atacadas por ela
    for (unsigned int row = 0; row < n; row++)
    {
        for (unsigned int col = 0; col < n; col++)
        {
            board[row] = col; // Coloca uma rainha na posição (row, col)

            // Verifica todas as outras posições no tabuleiro
            for (unsigned int i = 0; i < n; i++)
            {
                for (unsigned int j = 0; j < n; j++)
                {
                    // Pula a posição atual da rainha
                    if (i == row && j == col)
                        continue;

                    // Se a posição pode ser atacada, adiciona à lista de adjacência
                    if ((row == i || j == col || abs((int)(row - i)) == abs((int)(j - col))))
                    {
                        insere_fim_lista(i * n + j, &graph[row * n + col]);
                    }
                }
            }
        }
        // Remove a rainha da posição atual
        board[row] = 0;
    }
}

// Identifica quais vértices do grafo não estão bloqueados para montar o conjunto de vértices onde podemos inserir rainhas
void create_available_vertices(unsigned int graph_size, casa *c, unsigned int k, t_lista *available_vertices, unsigned int n)
{
    // Inicializa a lista de vértices disponíveis
    cria_lista(available_vertices);

    // Itera sobre todos os vértices do grafo
    for (unsigned int graph_vertice = 0; graph_vertice < graph_size; graph_vertice++)
    {
        int node_blocked = 0;
        unsigned int cell_blocked = 0;

        // Verifica se o vértice atual está em uma posição proibida
        while (cell_blocked < k && !node_blocked)
        {
            unsigned int line = c[cell_blocked].linha - 1;
            unsigned int column = c[cell_blocked].coluna - 1;

            if (line * n + column == graph_vertice)
                node_blocked = 1; // Marca o vértice como bloqueado se coincidir com uma posição proibida

            cell_blocked++;
        }

        // Se o vértice não estiver bloqueado, insere na lista de vértices disponíveis
        if (!node_blocked)
        {
            insere_fim_lista(graph_vertice, available_vertices);
        }
    }
}

// Função para liberar memória alocada para listas de adjacência do grafo, lista de vértices disponíveis e conjunto independente
void libera_listas(t_lista *graph, unsigned int graph_size, t_lista available_vertices, t_lista independet_set)
{
    // Libera cada lista de adjacência no grafo
    for (unsigned int i = 0; i < graph_size; i++)
    {
        destroi_lista(&graph[i]); // Libera a memória da lista de adjacência do nó i
    }
    free(graph); // Libera a memória alocada para o array de listas de adjacência

    // Libera a lista de vértices disponíveis
    destroi_lista(&available_vertices);

    // Libera a lista do conjunto independente
    destroi_lista(&independet_set);
}

// Função para encontrar um conjunto independente de vértices em um grafo
t_lista *ConjIndep(t_lista *graph, unsigned int n, t_lista *independent_set, t_lista *available_vertices)
{
    // Condição de parada: encontrou um conjunto independente de tamanho n
    if (independent_set->tamanho == n)
    {
        return independent_set;
    }

    // Condição de parada: não é possível formar um conjunto independente de tamanho n
    if (independent_set->tamanho + available_vertices->tamanho < n)
    {
        return NULL;
    }

    unsigned int v;
    // Remove o primeiro vértice disponível
    if (remove_primeiro_lista(&v, available_vertices) == 0)
    {
        return NULL;
    }

    // Cria uma cópia dos vértices disponíveis para backtracking
    t_lista *available_vertices_copy = (t_lista *)malloc(sizeof(t_lista));
    cria_lista(available_vertices_copy);
    copia_lista(available_vertices, available_vertices_copy);

    // Insere o vértice v + 1 no conjunto independente
    insere_fim_lista(v + 1, independent_set);

    // Remove os vizinhos de v dos vértices disponíveis
    Node *aux = graph[v].ini;
    while (aux != NULL)
    {
        unsigned int node_to_remove = aux->v;
        unsigned int item;

        if (pertence_lista(node_to_remove, available_vertices))
        {
            remove_item_lista(node_to_remove, &item, available_vertices);
        }

        aux = aux->next;
    }

    // Chamada recursiva para continuar construindo o conjunto independente
    t_lista *result = ConjIndep(graph, n, independent_set, available_vertices);

    if (result)
    {
        return result;
    }

    // Backtracking: restaura o estado anterior e tenta novamente sem o vértice v
    remove_item_lista(v + 1, &v, independent_set);

    return ConjIndep(graph, n, independent_set, available_vertices_copy);
}

// Função para resolver o problema das N-rainhas com conjuntos independentes
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

    // cria conjunto independente - conjunto solucao do problema
    t_lista independet_set, available_vertices;
    cria_lista(&independet_set);

    // criar conjunto available_vertices - conjunto que contém vértices não proibidos do grafo
    create_available_vertices(graph_size, c, k, &available_vertices, n);

    t_lista *result = ConjIndep(graph, n, &independet_set, &available_vertices);
    if (result)
    {
        Node *aux = result->ini;
        unsigned int i = 0;
        while (aux != NULL)
        {
            // retorna a coluna em que esta de acordo com o valor do vertice
            // -1 pois armazenamos a col+1 no tabuleiro e se for colocado na ultima coluna
            // o valor do vértice % n seria 0, e em seguida adicionamos 1 novamente ao valor da coluna
            unsigned int coluna = (aux->v - 1) % n;
            r[i] = coluna + 1;
            aux = aux->next;
            i++;
        }
    }

    free(board);
    libera_listas(graph, graph_size, available_vertices, independet_set);

    return r;
}
/**************************************************CONJUNTOS INDEPENDENTES***************************************************/

/**************************************************LISTAS***************************************************/
// Função para criar e inicializar uma lista ligada vazia
int cria_lista(t_lista *l)
{
    l->ini = NULL;  // Inicializa o início da lista como NULL
    l->tamanho = 0; // Inicializa o tamanho da lista como 0
    return 1;       // Retorna 1 indicando sucesso na criação da lista
}

// Função para inserir um novo nó com valor x no final de uma lista ligada
int insere_fim_lista(unsigned int x, t_lista *l)
{
    Node *new, *aux;

    // Se a lista estiver vazia, insere o nó no início
    if (lista_vazia(l))
        return insere_inicio_lista(x, l);

    // Aloca memória para um novo nó
    new = (Node *)malloc(sizeof(Node));
    if (new == NULL)
    {
        printf("Memória insuficiente\n");
        return 0; // Retorna 0 em caso de falha na alocação de memória
    }

    // Inicializa o novo nó
    new->v = x;
    new->next = NULL;
    aux = l->ini;

    // Percorre a lista até o último nó
    while (aux->next != NULL)
        aux = aux->next;

    // Incrementa o tamanho da lista
    l->tamanho++;
    // Define o ponteiro next do último nó para o novo nó
    aux->next = new;

    return 1; // Retorna 1 para indicar sucesso
}

// Função para verificar se uma lista está vazia
int lista_vazia(t_lista *l)
{
    if (l->ini == NULL) // Se o ponteiro inicial for NULL, a lista está vazia
        return 1;
    return 0; // Caso contrário, a lista não está vazia
}

// Função para inserir um novo nó com valor x no início de uma lista ligada
int insere_inicio_lista(unsigned int x, t_lista *l)
{
    Node *new;

    // Aloca memória para um novo nó
    new = (Node *)malloc(sizeof(Node));
    if (new == NULL)
    {
        printf("Memória insuficiente\n");
        return 0; // Retorna 0 em caso de falha na alocação de memória
    }

    // Inicializa o novo nó
    new->v = x;
    new->next = NULL;
    l->tamanho++;

    // Verifica se a lista está vazia
    if (lista_vazia(l))
    {
        l->ini = new; // Se a lista estiver vazia, o novo nó é o primeiro nó
        return 1;     // Retorna 1 para indicar sucesso
    }

    // Se a lista não estiver vazia, insere o novo nó no início
    new->next = l->ini;
    l->ini = new;

    return 1; // Retorna 1 para indicar sucesso
}

// Função para remover o primeiro nó de uma lista ligada e armazenar seu valor na variável fornecida
unsigned int remove_primeiro_lista(unsigned int *item, t_lista *l)
{
    Node *aux;

    // Verifica se a lista está vazia
    if (lista_vazia(l))
        return 0; // Retorna 0 se a lista estiver vazia

    // Caso a lista não esteja vazia
    l->tamanho--;          // Decrementa o tamanho da lista
    *item = l->ini->v;     // Armazena o valor do primeiro nó em item
    aux = l->ini;          // Guarda o endereço do primeiro nó em aux
    l->ini = l->ini->next; // Atualiza o ponteiro inicial da lista para o próximo nó
    free(aux);             // Libera a memória do nó removido

    return 1; // Retorna 1 para indicar sucesso na remoção
}

// Função para remover um nó específico com valor v de uma lista ligada
unsigned int remove_item_lista(unsigned int v, unsigned int *item, t_lista *l)
{
    Node *aux, *anterior;

    // Verifica se a lista está vazia
    if (lista_vazia(l))
        return 0; // Retorna 0 se a lista estiver vazia

    // Caso o elemento esteja na primeira posição
    if (l->ini->v == v)
    {
        return remove_primeiro_lista(item, l); // Remove o primeiro nó se ele tiver o valor v
    }

    // Percorre a lista para encontrar o nó com valor v
    aux = l->ini->next;
    anterior = l->ini;
    while ((aux->next != NULL) && (aux->v != v))
    {
        anterior = aux;
        aux = aux->next;
    }

    // Caso tenha chegado ao fim da lista e o elemento não foi achado
    if ((aux->next == NULL) && (aux->v != v))
    {
        printf("Nao removido: elemento nao encontrado.\n");
        return 0; // Retorna 0 se o nó não for encontrado
    }
    // Caso o elemento esteja na última posição
    else if ((aux->next == NULL) && (aux->v == v))
    {
        *item = aux->v;        // Armazena o valor do nó a ser removido
        anterior->next = NULL; // Remove o nó ajustando o ponteiro do nó anterior
    }
    // Caso o elemento esteja no meio da lista
    else
    {
        *item = aux->v;             // Armazena o valor do nó a ser removido
        anterior->next = aux->next; // Remove o nó ajustando o ponteiro do nó anterior
    }

    l->tamanho--; // Decrementa o tamanho da lista
    free(aux);    // Libera a memória do nó removido
    return 1;     // Retorna 1 para indicar sucesso na remoção
}

// Função para verificar se um valor v está presente em uma lista ligada
int pertence_lista(unsigned int v, t_lista *l)
{
    Node *aux;

    // Verifica se a lista está vazia
    if (lista_vazia(l))
        return 0; // Retorna 0 se a lista estiver vazia

    aux = l->ini; // Inicia o ponteiro auxiliar no início da lista
    // Percorre a lista até encontrar o valor v ou chegar ao fim
    while ((aux->next != NULL) && (aux->v != v))
        aux = aux->next;

    // Verifica se o valor foi encontrado
    if (aux->v == v)
        return 1; // Retorna 1 se o valor v estiver presente na lista

    return 0; // Retorna 0 se o valor v não estiver presente na lista
}

// Função para destruir uma lista ligada e liberar a memória alocada
void destroi_lista(t_lista *l)
{
    // Verifica se a lista está vazia
    if (lista_vazia(l))
    {
        return; // Retorna imediatamente se a lista estiver vazia
    }

    Node *aux = l->ini;      // Ponteiro para o nó atual
    Node *aux_2 = aux->next; // Ponteiro para o próximo nó

    // Percorre a lista e libera a memória de cada nó
    while (aux_2 != NULL)
    {
        free(aux);           // Libera a memória do nó atual
        aux = aux_2;         // Atualiza aux para o próximo nó
        aux_2 = aux_2->next; // Atualiza aux_2 para o próximo nó
    }
    free(aux); // Libera a memória do último nó

    l->tamanho = 0; // Redefine o tamanho da lista para 0
    l->ini = NULL;  // Define o ponteiro inicial como NULL, indicando que a lista está vazia
}

// Função para copiar todos os elementos de lista1 para lista2
int copia_lista(t_lista *lista1, t_lista *lista2)
{
    Node *aux_l;

    // Verifica se a lista1 está vazia
    if (lista_vazia(lista1))
        return 0; // Retorna 0 se lista1 estiver vazia

    aux_l = lista1->ini;                   // Inicializa o ponteiro auxiliar no início de lista1
    insere_inicio_lista(aux_l->v, lista2); // Insere o valor do primeiro nó no início de lista2

    // Percorre lista1 a partir do segundo nó
    while (aux_l->next != NULL)
    {
        aux_l = aux_l->next;                // Atualiza aux_l para o próximo nó
        insere_fim_lista(aux_l->v, lista2); // Insere o valor no final de lista2
    }

    // Atualiza o tamanho de lista2
    lista2->tamanho = lista1->tamanho;

    return 1; // Retorna 1 para indicar sucesso na cópia
}
/**************************************************LISTAS***************************************************/