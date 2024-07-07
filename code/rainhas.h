#ifndef RAINHAS_H
#define RAINHAS_H

// Estrutura para uma casa do tabuleiro
typedef struct casa
{
  unsigned int linha, coluna;
} casa;

// Estrutura para nós do grafo
struct Node
{
  unsigned int v;
  struct Node *next;
};
typedef struct Node Node;

// Estrutura para listas
struct t_lista
{
  Node *ini;
  unsigned int tamanho;
};
typedef struct t_lista t_lista;

/**************************************************LISTAS***************************************************/
int cria_lista(t_lista *l);
int insere_fim_lista(unsigned int x, t_lista *l);
int lista_vazia(t_lista *l);
int insere_inicio_lista(unsigned int x, t_lista *l);
unsigned int remove_primeiro_lista(unsigned int *item, t_lista *l);
unsigned int remove_item_lista(unsigned int chave, unsigned int *item, t_lista *l);
int pertence_lista(unsigned int chave, t_lista *l);
void destroi_lista(t_lista *l);
int copia_lista(t_lista *lista1, t_lista *lista2);
/**************************************************LISTAS***************************************************/

/**************************************************BACKTRACKING***************************************************/
int is_safe(unsigned int *board, unsigned int row, unsigned int col, casa *c, unsigned int k);
int solve_n_queens_bt(unsigned int *board, unsigned int row, unsigned int n, casa *c, unsigned int k);
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
/**************************************************BACKTRACKING***************************************************/

/**************************************************CONJUNTOS INDEPENDENTES***************************************************/
void create_graph(unsigned int *board, t_lista *graph, unsigned int graph_size, unsigned int n);
void create_available_vertices(unsigned int graph_size, casa *c, unsigned int k, t_lista *available_vertices, unsigned int n);
void libera_listas(t_lista *graph, unsigned int graph_size, t_lista available_vertices, t_lista independet_set);
t_lista *ConjIndep(t_lista *graph, unsigned int n, t_lista *independent_set, t_lista *available_vertices);
//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n rainhas
// com casas proibidas usando backtracking
// n, c, r e o valor de retorno são como em rainhas_bt
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r);
/**************************************************CONJUNTOS INDEPENDENTES***************************************************/
#endif
