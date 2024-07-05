#ifndef RAINHAS_H
#define RAINHAS_H

// Representa uma casa do tabuleiro
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

struct t_lista {
    Node *ini;
    int tamanho;
};
typedef struct t_lista t_lista;

// Protótipos
int is_safe(unsigned int* board, unsigned int row, unsigned int col, casa* c, unsigned int k);
int solve_n_queens_bt(unsigned int *board, unsigned int row, unsigned int n, casa *c, unsigned int k);
int solve_n_queens_ci(unsigned int* board, unsigned int row, unsigned int n, casa* c, unsigned int k);
void create_graph(unsigned int *board, t_lista* graph, unsigned int graph_size, unsigned int n);
// IMPLEMENTACOES DA LISTA
void imprime_lista (t_lista *l);
int cria_lista(t_lista *l);
int insere_fim_lista(unsigned int x, t_lista *l);
int lista_vazia(t_lista *l);
int insere_inicio_lista(unsigned int x, t_lista *l);

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

#endif
