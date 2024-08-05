====================================
Documentação das Funções e Estruturas
====================================

Autores: 
    - Gabriel Razzolini Pires De Paula -> GRR20197155
    - Erick Graeff Petzold -> GRR

Como rodar:
    - Para executar o programa primeiro compile-o usando o comando "make"
    - execute da seguinte maneira:
      ./teste < <caminho_para_arquivo>/<arquivo>

OBS.: Há um define no inicio do arquivo chamado "#define GRAPH_INFO 1". Este serve para mostrar prints que dão informações mais completas sobre o grafo e sua estrutura.
Caso queira, ele serve para ter mais informações sobre o grafo em questão como por exemplo:
    - sua lista de adjacência
    - os seus pontos de articulação(vértices de corte)
    - quais vértices são adicionados em cada bloco
    - e por fim informações gerais sobre os blocos, apenas deve-se descomentar a linha que faz o define

------------------------------------
Structs
------------------------------------
1. AdjListNode
   - Descrição: Estrutura para representar um nó de lista de adjacência.
   - Campos:
     - int dest: Destino do nó.
     - AdjListNode* next: Ponteiro para o próximo nó da lista.

2. AdjList
   - Descrição: Estrutura para representar uma lista de adjacência.
   - Campos:
     - AdjListNode* head: Ponteiro para o nó cabeça da lista.

3. Graph
   - Descrição: Estrutura para representar um grafo.
   - Campos:
     - int V: Número de vértices no grafo.
     - AdjList* array: Array de listas de adjacência.

------------------------------------
Funções
------------------------------------
1. AdjListNode* newAdjListNode(int dest)
   - Descrição: Cria um novo nó de lista de adjacência.
   - Parâmetros:
     - int dest: Destino do nó.
   - Retorno: Ponteiro para o novo nó criado.

2. void addEdge(Graph* graph, int src, int dest)
   - Descrição: Adiciona uma aresta a um grafo não direcionado.
   - Parâmetros:
     - Graph* graph: Ponteiro para o grafo.
     - int src: Vértice de origem.
     - int dest: Vértice de destino.

3. Graph* createGraph(int V)
   - Descrição: Cria um grafo com "V" vértices.
   - Parâmetros:
     - int V: Número de vértices.
   - Retorno: Ponteiro para o grafo criado.

4. void freeGraph(Graph* graph)
   - Descrição: Libera a memória alocada para um grafo.
   - Parâmetros:
     - Graph* graph: Ponteiro para o grafo.

5. Graph* readInput(void)
   - Descrição: Lê a entrada formatada da stdin e cria um grafo.
   - Parâmetros: Nenhum.
   - Retorno: Ponteiro para o grafo criado.

6. void DFSarticulationPoints(Graph* graph, int u, bool visited[], int disc[], int low[], int parent[], bool isArticulationPoint[], int* time)
   - Descrição: Realiza uma busca em profundidade (DFS) para encontrar pontos de articulação(vértices de corte).
   - Parâmetros:
     - Graph* graph: Ponteiro para o grafo.
     - int u: Vértice atual.
     - bool visited[]: Array de vértices visitados.
     - int disc[]: Array de tempos de descoberta.
     - int low[]: Array de menores tempos de descoberta.
     - int parent[]: Array de pais dos vértices.
     - bool isArticulationPoint[]: Array de pontos de articulação(vértices de corte).
     - int* time: Ponteiro para o tempo atual.

7. void findAllArticulationPoints(Graph* graph, bool* isArticulationPoint)
   - Descrição: Encontra e imprime todos os pontos de articulação(vértices de corte) no grafo.
   - Parâmetros:
     - Graph* graph: Ponteiro para o grafo.
     - bool* isArticulationPoint: Array de pontos de articulação(vértices de corte).

8. void BFSblocks(Graph* graph, int startVertices, bool* visited, bool* isArticulationPoint, int* blockSize, bool* blockMembers)
   - Descrição: Realiza uma busca em largura (BFS) para encontrar os vértices e arestas em um bloco.
   - Parâmetros:
     - Graph* graph: Ponteiro para o grafo.
     - int startVertices: Vértice inicial.
     - bool* visited: Array de vértices visitados.
     - bool* isArticulationPoint: Array de pontos de articulação(vértices de corte).
     - int* blockSize: Ponteiro para o tamanho do bloco.
     - bool* blockMembers: Array de membros do bloco.

9. info_t* findAllBlocks(Graph* graph, unsigned int* num_blocos)
   - Descrição: Encontra todos os blocos no grafo.
   - Parâmetros:
     - Graph* graph: Ponteiro para o grafo.
     - unsigned int* num_blocos: Ponteiro para o número de blocos encontrados.
   - Retorno: Ponteiro para a informação dos blocos.

10. info_t* info_blocos(unsigned int* num_blocos)
    - Descrição: Lê a entrada e encontra todos os blocos no grafo.
    - Parâmetros:
      - unsigned int* num_blocos: Ponteiro para o número de blocos encontrados.
    - Retorno: Ponteiro para a informação dos blocos.
