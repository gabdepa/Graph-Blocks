# Projeto: Solução do Problema das N Rainhas

Este código contém a implementação em C para resolver o problema das N rainhas, onde o objetivo é posicionar N rainhas em um tabuleiro de xadrez de tamanho N x N de forma que nenhuma rainha esteja atacando outra.

### Estruturas de Dados
- **$Node$**: Estrutura para os nós utilizados na lista de adjacências do grafo.
        **v**: Valor do vértice no nó.
        **next**: Ponteiro para o próximo nó na lista.

- **$t_lista$**: Estrutura para representar uma lista.
        **ini**: Ponteiro para o primeiro nó da lista.
        **tamanho**: Representa o tamanho da lista.

## Funções   
- **$is_safe$**
    - **Descrição**: Verifica se é seguro colocar uma rainha na posição indicada no tabuleiro, considerando as rainhas já posicionadas.
    - **Parâmetros**: unsigned int* board, unsigned int row, unsigned int col, casa* c, unsigned int k
    - **Retorno**: Retorna 1 se for seguro, 0 caso contrário.

- **$solve_n_queens_bt$**
    - **Descrição**: Soluciona o problema das N rainhas usando backtracking, tentando posicionar rainhas linha por linha.
    - **Parâmetros**: unsigned int *board, unsigned int row, unsigned int n, casa *c, unsigned int k
    - **Retorno**: Retorna 1 se uma solução for encontrada, 0 caso contrário.

- **$solve_n_queens_ci$**
    - **Descrição**: Soluciona o problema das N rainhas usando outra técnica (possivelmente conjuntos independentes), tentando posicionar rainhas linha por linha.
    - **Parâmetros**: unsigned int* board, unsigned int row, unsigned int n, casa* c, unsigned int k
    - **Retorno**: Retorna 1 se uma solução for encontrada, 0 caso contrário.

- **$create_graph$**
    - **Descrição**: Cria um grafo baseado no estado atual do tabuleiro para mapear as relações entre as posições das rainhas.
    - **Parâmetros**: unsigned int *board, t_lista* graph, unsigned int graph_size, unsigned int n

- **$create_available_vertices$**
    - **Descrição**: Popula uma lista com vértices disponíveis que podem ser escolhidos sem entrar em conflito com as rainhas já posicionadas.
    - **Parâmetros**: unsigned int graph_size, casa *c, unsigned int k, t_lista *available_vertices, unsigned int n

- **$ConjIndep$**
    - **Descrição**: Busca um conjunto independente no grafo que representa as posições válidas das rainhas.
    - **Parâmetros**: t_lista *graph, unsigned int n, t_lista *independent_set, t_lista *available_vertices

- **$imprime_lista$**
    - **Descrição**: Imprime os elementos de uma lista.
    - **Parâmetros**: t_lista *l

- **$cria_lista$**
    - **Descrição**: Inicializa uma nova lista.
    - **Parâmetros**: t_lista *l
    - **Retorno**: Retorna 0 em sucesso.

- **$insere_fim_lista$**
    - **Descrição**: Insere um elemento no final de uma lista.
    - **Parâmetros**: unsigned int x, t_lista *l
    - **Retorno**: Retorna 1 em sucesso, 0 em falha.

- **$remove_primeiro_lista$**
    - **Descrição**: Remove o primeiro item da lista e retorna seu valor.
    - **Parâmetros**: unsigned int *item, t_lista *l

- **$remove_item_lista$**
    - **Descrição**: Remove um item específico da lista e retorna seu valor.
    - **Parâmetros**: unsigned int v, unsigned int *item, t_lista *l

- **$lista_vazia$**
    - **Descrição**: Verifica se a lista está vazia.
    - **Parâmetros**: t_lista *l
    - **Retorno**: Retorna 1 se a lista estiver vazia, 0 caso contrário.

- **$insere_inicio_lista$**
    - **Descrição**: Insere um elemento no início de uma lista.
    - **Parâmetros**: unsigned int x, t_lista *l
    - **Retorno**: Retorna 1 em sucesso, 0 em falha.

- **$destroi_lista$**
    - **Descrição**: Libera todos os recursos associados a uma lista.
    - **Parâmetros**: t_lista *l

- **$libera_listas$**
    - **Descrição**: Libera um conjunto de listas utilizadas no programa.
    - **Parâmetros**: t_lista *graph, unsigned int graph_size, t_lista available_vertices, t_lista independent_set

- **$pertence_lista$**
    - **Descrição**: Verifica se um item pertence à lista.
    - **Parâmetros**: unsigned int chave, t_lista *l
    - **Retorno**: Retorna 1 se o item pertence, 0 caso contrário.

- **$insere_ordenado_lista$**
    - **Descrição**: Insere um elemento de maneira ordenada em uma lista.
    - **Parâmetros**: unsigned int x, t_lista *l
    - **Retorno**: Retorna 1 em sucesso, 0 em falha.

- **$copia_lista$**
    - **Descrição**: Copia os elementos de uma lista para outra.
    - **Parâmetros**: t_lista *lista1, t_lista *lista2
    - **Retorno**: Retorna 0 em sucesso.