# Projeto: Solução do Problema das N Rainhas

Este código contém a implementação em C para resolver o problema das N rainhas, onde o objetivo é posicionar N rainhas em um tabuleiro de xadrez de tamanho N x N de forma que nenhuma rainha esteja atacando outra.

### Estruturas de Dados
- **`Node`**: Estrutura para os nós utilizados na lista de adjacências do grafo.
        **v**: Valor do vértice no nó.
        **next**: Ponteiro para o próximo nó na lista.

- **`t_lista`**: Estrutura para representar uma lista.
        **ini**: Ponteiro para o primeiro nó da lista.
        **tamanho**: Representa o tamanho da lista.

## Funções   

### BackTracking
**`is_safe()`**
- **Descrição**: A função verifica se é seguro colocar uma rainha em uma posição específica em um tabuleiro de xadrez, levando em conta tanto as posições proibidas quanto as posições das rainhas já colocadas.
- **Lógica**: A função realiza 2 verificações principais:
    1. **Casas Proibidas**: Percorre uma lista de posições proibidas (`c`), representada por um array de estruturas `casa`, e verifica se a posição desejada (`row`, `col`) coincide com alguma dessas posições proibidas.
    2. **Ataques de Outras Rainhas**: Verifica se a nova posição entra em conflito com as rainhas já colocadas no tabuleiro. 
    Se nenhuma dessas condições de conflito for encontrada, a função retorna 1, indicando que é seguro colocar a rainha. Caso contrário, retorna 0.
- **Parâmetros**: 
    - `unsigned int *board`: Um array onde cada índice representa uma linha do tabuleiro e o valor armazenado representa a coluna em que uma rainha está colocada.
    - `unsigned int row`: A linha onde se deseja colocar a nova rainha.
    - `unsigned int col`: A coluna onde se deseja colocar a nova rainha.
    - `casa *c`: Um array de estruturas casa que contém posições proibidas no tabuleiro.
    - `unsigned int k`: O número de posições proibidas no array c.
- **Retorno**: Retorna 1 se for seguro, 0 caso contrário.

**`solve_n_queens_bt()`**
- **Descrição**: A função é uma função auxiliar que utiliza a técnica de backtracking para resolver o problema das N-rainhas. O objetivo do problema é colocar N rainhas em um tabuleiro de N x N de forma que nenhuma rainha possa atacar outra.
- **Lógica**: A função segue uma abordagem recursiva e de backtracking:
    1. **Condição de parada**: Se todas as rainhas foram colocadas (`row >= n`), a função retorna 1, indicando que uma solução foi encontrada.
    2. **Tentativa de colocar uma rainha**: Para cada coluna em uma linha (`for` loop), a função tenta colocar uma rainha na posição `(row, i)`.
        - **Verificação de segurança**: Usa a função `is_safe` para verificar se é seguro colocar a rainha na posição atual.
        - **Colocação da rainha**: Se for seguro, a rainha é colocada (`board[row] = i + 1`).
        - **Chamada recursiva**: A função chama a si mesma para tentar colocar a próxima rainha na linha seguinte `(row + 1)`.
        - **Backtracking**: Se a chamada recursiva retornar 0 (indicando que não foi possível encontrar uma solução com a rainha na posição atual), a função remove a rainha (backtrack) e tenta a próxima coluna.
    3. **Retorno de falha**: Se nenhuma coluna na linha atual permitir uma solução, a função retorna 0, indicando que não foi possível resolver o problema com a configuração atual.
- **Parâmetros**: 
    - `unsigned int *board`: Um ponteiro para um array que representa o tabuleiro. Cada índice representa uma linha e o valor em cada posição do array representa a coluna onde uma rainha está colocada.
    - `unsigned int row`: A linha atual onde se tenta colocar uma rainha.
    - `unsigned int n`: O tamanho do tabuleiro (e o número de rainhas a serem colocadas).
    - `casa *c`: Um ponteiro para uma estrutura que contém posições proibidas onde uma rainha não pode ser colocada.
    - `unsigned int k`: O número de posições proibidas contidas na estrutura c.
- **Retorno**: Retorna 1 se uma solução for encontrada, 0 caso contrário.

### Conjuntos Independentes
**`ConjIndep()`**
- **Descrição**: A função busca encontrar um conjunto independente de tamanho `n` em um grafo representado por uma lista de adjacência. Um conjunto independente é um conjunto de vértices em que nenhum vértice está diretamente conectado a outro vértice do conjunto.
- **Lógica**: 
    1. **Verificação de Tamanho do Conjunto Independente**: Se o tamanho do conjunto independente (`independent_set->tamanho`) é igual a `n`, a função retorna `independent_set`, indicando que encontrou um conjunto independente de tamanho `n`.
    2. **Verificação de Viabilidade**: Se a soma do tamanho do conjunto independente e dos vértices disponíveis (`independent_set->tamanho + available_vertices->tamanho`) é menor que `n`, retorna `NULL`, indicando que não é possível formar um conjunto independente de tamanho `n` com os vértices restantes.
    3. **Remoção e Processamento do Primeiro Vértice Disponível**: Remove o primeiro vértice disponível (`v`) de `available_vertices`. Se falhar (não há vértices disponíveis), retorna `NULL`.
    4. **Cópia dos Vértices Disponíveis**: Cria uma cópia de `available_vertices` para possibilitar a reversão do estado posteriormente, se necessário.
    5. **Inserção do Vértice no Conjunto Independente**: Insere o vértice `v + 1` no final de `independent_set`.
    6. **Remoção dos Vizinhos do Vértice dos Vértices Disponíveis**: Remove todos os vizinhos de `v` de `available_vertices`.
    7. **Chamada Recursiva**: 
        - Faz uma chamada recursiva à função `ConjIndep` com o conjunto independente atualizado e os vértices disponíveis atualizados.
        - Se a chamada recursiva retorna um resultado válido, retorna esse resultado.
    8. **Reversão do Estado e Nova Chamada Recursiva**:
        - Se a chamada recursiva anterior falhou, remove `v + 1` de `independent_set` para reverter o estado.
        - Faz uma nova chamada recursiva à função `ConjIndep` usando a cópia dos vértices disponíveis, sem incluir `v`.
- **Parâmetros**:
    - `t_lista *graph`: Um array de listas de adjacência que representa o grafo.
    - `unsigned int n`: O tamanho do conjunto independente desejado.
    - `t_lista *independent_set`: Uma lista que armazena os vértices do conjunto independente atual.
    - `t_lista *available_vertices`: Uma lista de vértices que ainda estão disponíveis para inclusão no conjunto independente.
- **Retorno**: Retorna um ponteiro para `t_lista` que contém o conjunto independente de tamanho `n` se for encontrado. Caso contrário, retorna`NULL` se não for possível formar um conjunto independente de tamanho `n` com os vértices disponíveis.


**`create_graph()`**
- **Descrição**: A função cria um grafo representando o problema das N-rainhas. Cada posição no tabuleiro é um nó no grafo, e arestas são criadas entre nós que representam posições do tabuleiro onde uma rainha pode atacar outra.
- **Lógica**:
    1. **Inicialização das Listas**: Para cada entrada no array `graph`, inicializa uma lista vazia.
    2. **Iteração pelo Tabuleiro**: Itera sobre cada posição no tabuleiro para colocar uma rainha e identificar posições que seriam atacadas por ela.
    3. **Verificação de Ataques**: Para cada posição do tabuleiro, verifica todas as outras posições e determina se a posição atual poderia atacar outra posição. Se puder, adiciona essa posição à lista de adjacência do nó correspondente.
    4. **Remoção Temporária da Rainha**: Após verificar uma posição, remove a rainha (seta a posição no `board` para 0).
- **Parâmetros**: 
    - `unsigned int *board`: Um array que representa o tabuleiro de xadrez. Cada índice representa uma linha e o valor em cada posição do array representa a coluna onde uma rainha está colocada.
    - `t_lista *graph`: Um array de listas de adjacência. Cada entrada representa um nó do grafo e contém uma lista de nós adjacentes (posições atacadas).
    - `unsigned int graph_size`: O tamanho do grafo, que é igual ao número de posições no tabuleiro (N x N).
    - `unsigned int n`: O tamanho do tabuleiro (e o número de rainhas a serem colocadas).
- **Retorno**: A função não retorna um valor. Em vez disso, ela modifica diretamente o grafo passado por referência.

**`create_available_vertices()`**
- **Descrição**: A função identifica quais vértices em um grafo não estão bloqueados, criando uma lista dos vértices onde é possível inserir rainhas. A função leva em consideração um conjunto de posições proibidas.
    - **Lógica**: 
    1. **Inicialização da Lista**: Inicializa a lista `available_vertices` que vai armazenar os vértices disponíveis.
    2. **Iteração pelos Vértices do Grafo**: Para cada vértice no grafo, verifica se o vértice está bloqueado com base nas posições  proibidas.
    3. **Verificação de Bloqueio**:
        - Para cada posição proibida (armazenada na estrutura `c`), calcula a posição correspondente no grafo.
        - Se o vértice atual coincide com uma posição proibida, marca o vértice como bloqueado.
    4. **Inserção na Lista de Vértices Disponíveis**: Se o vértice não estiver bloqueado, insere-o na lista `available_vertices`.
- **Parâmetros**: 
    - `unsigned int graph_size`: O número total de vértices no grafo.
    - `casa *c`: Um array de estruturas casa que contém as posições proibidas no tabuleiro.
    - `unsigned int k`: O número de posições proibidas.
    - `t_lista *available_vertices`: Uma lista onde serão armazenados os vértices disponíveis.
    - `unsigned int n`: O tamanho do tabuleiro (nxn).
- **Retorno**: A função não retorna um valor. Em vez disso, ela modifica diretamente a lista `available_vertices` passada por referência, inserindo os vértices que não estão bloqueados.

**`libera_listas()`**
- **Descrição**: A função é responsável por liberar a memória alocada para as listas de adjacência de um grafo, além de liberar as listas de vértices disponíveis e o conjunto independente.
- **Lógica**: 
    1. **Liberação das Listas de Adjacência do Grafo**:
        - Itera sobre cada lista no array `graph`.
        - Para cada lista, chama a função `destroi_lista` para liberar a memória alocada para os nós dessa lista.
    2. **Liberação do Array de Listas de Adjacência**: Libera a memória alocada para o array `graph` utilizando a função `free()`.
    3. **Liberação das Listas de Vértices Disponíveis e Conjunto Independente**:
        - Chama a função `destroi_lista` para liberar a memória alocada para a lista `available_vertices`.
        - Chama a função `destroi_lista` para liberar a memória alocada para a lista `independet_set`.
- **Parâmetros**: 
    - `t_lista *graph`: Um ponteiro para um array de listas de adjacência do grafo. Cada entrada representa um nó do grafo e contém uma lista de nós adjacentes.
    - `unsigned int graph_size`: O número total de listas no grafo (tamanho do array graph).
    - `t_lista available_vertices`: Uma lista que contém os vértices disponíveis.
    - `t_lista independet_set`: Uma lista que contém o conjunto independente de vértices.
- **Retorno**: A função não retorna um valor. Ela libera a memória alocada para as estruturas de dados passadas como parâmetro.


### Funções auxiliares de Listas
**`cria_lista()`**
- **Descrição**: A função inicializa uma lista ligada vazia. A lista é representada pela estrutura `t_lista`, que contém um ponteiro para o início da lista e um campo para armazenar seu tamanho.
- **Lógica**: 
    1. **Inicialização dos Campos da Estrutura**:
        - Define o ponteiro `ini` como `NULL`, indicando que a lista está vazia.
        - Define o campo `tamanho` como `0`, indicando que a lista inicialmente não contém elementos.
    2. **Retorno de Sucesso**: Retorna `1` para indicar que a lista foi criada com sucesso.
- **Parâmetros**: 
    - `t_lista *l`: Um ponteiro para a estrutura `t_lista` que será inicializada como uma lista vazia.
- **Retorno**: Retorna `1` para indicar que a lista foi criada com sucesso.

**`insere_fim_lista()`**
- **Descrição**: A função  insere um novo nó com o valor `x` no final de uma lista ligada representada pela estrutura `t_lista`. Se a lista estiver vazia, a função chama outra função para inserir o nó no início.
- **Lógica**: 
    1. **Verificação se a Lista está Vazia**: Se a lista estiver vazia, a função chama insere_inicio_lista para inserir o nó no início da lista.
    2. **Criação de um Novo Nó**: 
        - Aloca memória para um novo nó (`new`).
        - Se a alocação falhar, imprime uma mensagem de erro e retorna `0`.
        - Inicializa o novo nó com o valor `x` e define seu ponteiro `next` como `NULL`.
    3. **Percorrer a Lista até o Fim**:
        - Define um ponteiro auxiliar (`aux`) para percorrer a lista.
        - Percorre a lista até encontrar o último nó (onde `next` é `NULL`).
    4. **Inserção do Novo Nó**:
        - Incrementa o tamanho da lista.
        - Define o ponteiro `next` do último nó para apontar para o novo nó.
- **Parâmetros**: 
    - `unsigned int x`: O valor a ser inserido no novo nó. 
    - `t_lista *l`: Um ponteiro para a estrutura `t_lista` onde o novo nó será inserido.
- **Retorno**: Retorna `1` para indicar que o nó foi inserido com sucess, `0` caso contrário.

**`lista_vazia()`**
- **Descrição**: A função verifica se uma lista ligada está vazia.
- **Lógica**:
    1. **Verificação do Ponteiro Inicial**: Se o ponteiro `ini` da estrutura `t_lista` for `NULL`, a lista está vazia, caso contrário, a lista contém elementos.
- **Parâmetros**:
    - `t_lista *l`: Um ponteiro para a estrutura `t_lista` que representa a lista a ser verificada.
- **Retorno**: Retorna `1` se a lista estiver vazia, `0` caso contrário (a lista contém elementos)

**`insere_inicio_lista()`**
- **Descrição**: A função insere um novo nó com o valor `x` no início de uma lista ligada representada pela estrutura `t_lista`.
- **Lógica**: 
    1. **Alocação de Memória para um Novo Nó**:
        - Aloca memória para um novo nó (`new`).
        - Se a alocação falhar, imprime uma mensagem de erro e retorna `0`.
    2. **Inicialização do Novo Nó**:
        - Define o valor do nó como `x`.
        - Define o ponteiro `next` do nó como `NULL`.
        - Incrementa o tamanho da lista.
    3. **Inserção do Nó no Início da Lista**:
        - Verifica se a lista está vazia utilizando a função `lista_vazia`.
        - Se a lista estiver vazia, define o ponteiro inicial da lista (`ini`) para o novo nó.
        - Se a lista não estiver vazia, ajusta o ponteiro `next` do novo nó para apontar para o nó atual no início da lista e atualiza o ponteiro inicial da lista para o novo nó.
- **Parâmetros**: 
    - `unsigned int x`: O valor a ser inserido no novo nó.
    - `t_lista *l`: Um ponteiro para a estrutura `t_lista` onde o novo nó será inserido.
- **Retorno**: Retorna `1` para indicar que o nó foi inserido com sucesso, `0` em caso de falha na alocação da memória.

**`remove_primeiro_lista()`**
- **Descrição**: A função remove o primeiro nó de uma lista ligada, representada pela estrutura `t_lista`, e armazena o valor do nó removido em uma variável fornecida pelo usuário.
- **Lógica**:
    1. **Verificação se a Lista está Vazia**:
        - A função verifica se a lista está vazia utilizando a função `lista_vazia`.
        - Se a lista estiver vazia, retorna `0`, indicando que não há elementos para remover.
    2. **Remoção do Primeiro Nó**:
        - Decrementa o tamanho da lista.
        - Armazena o valor do primeiro nó (`l->ini->v`) na variável fornecida (`item`).
        - Cria um ponteiro auxiliar (`aux`) para armazenar o endereço do primeiro nó.
        - Atualiza o ponteiro inicial da lista para apontar para o próximo nó (`l->ini->next`).
        - Libera a memória alocada para o nó removido.
- **Parâmetros**: 
    - `unsigned int *item`: Um ponteiro para uma variável onde será armazenado o valor do nó removido.
    - `t_lista *l`: Um ponteiro para a estrutura `t_lista` de onde o nó será removido.
- **Retorno**: Retorna `1` se o nó for removido com sucesso, `0` se a lista estiver vazia e não houver elementos para remover.


**`remove_item_lista()`**
- **Descrição**: A função busca e remove um nó específico com valor `v` de uma lista ligada representada pela estrutura `t_lista`. Se o nó for encontrado e removido, o valor é armazenado na variável fornecida.
- **Lógica**:
    1. **Verificação se a Lista está Vazia**:
        - A função verifica se a lista está vazia usando `lista_vazia`.
        - Se estiver vazia, retorna `0` indicando que não há elementos para remover.
    2. **Remoção do Primeiro Nó**: Se o nó a ser removido está na primeira posição (ou seja, `l->ini->v == v`), a função chama `remove_primeiro_lista` para remover o nó e retorna o resultado dessa operação.
    3. **Busca do Nó na Lista**:
        - A função percorre a lista com dois ponteiros: `aux` (o nó atual) e `anterior` (o nó anterior).
        - Se encontrar o nó com valor `v`, remove-o e ajusta os ponteiros da lista.
    4. **Verificação de Condições Especiais**:
        - Se o nó não for encontrado, imprime uma mensagem e retorna `0`.
        - Se o nó estiver na última posição, ajusta o ponteiro `next` do nó anterior para `NULL`.
        - Se o nó estiver no meio da lista, ajusta o ponteiro `next` do nó anterior para pular o nó removido.
    5. **Liberação de Memória e Atualização da Lista**:
        - Decrementa o tamanho da lista.
        - Libera a memória do nó removido.
- **Parâmetros**: 
    - `unsigned int v`: O valor do nó a ser removido.
    - `unsigned int *item`: Um ponteiro para uma variável onde será armazenado o valor do nó removido.
    - `t_lista *l`: Um ponteiro para a estrutura `t_lista` de onde o nó será removido.
- **Retorno**: Retorna `1` se o nó for removido com sucesso. `0` se a lista estiver vazia ou se o nó não for encontrado.

**`pertence_lista()`**
- **Descrição**: A função verifica se um valor `v` está presente em uma lista ligada representada pela estrutura `t_lista`.
- **Lógica**: 
    1. **Verificação se a Lista está Vazia**:
        - A função utiliza `lista_vazia` para verificar se a lista está vazia.
        - Se a lista estiver vazia, retorna `0`, indicando que o valor não está presente.
    2. **Percorrer a Lista**:
        - Inicia um ponteiro auxiliar (`aux`) no início da lista (`l->ini`).
        - Percorre a lista enquanto o nó atual não for o último nó e seu valor (`aux->v`) não for igual a `v`.
    3. **Verificação do Valor**:
        - Após percorrer a lista, se o valor do nó atual (`aux->v`) for igual a `v`, retorna `1`, indicando que o valor está presente na lista.
        - Se o valor não for encontrado, retorna `0`.
- **Parâmetros**: 
    - `unsigned int v`: O valor a ser verificado na lista.
    - `t_lista *l`: Um ponteiro para a estrutura `t_lista` onde será feita a verificação.
- **Retorno**: Retorna `1` se o valor `v` estiver presente na lista. `0` se o valor `v` não estiver presente na lista ou se a lista estiver vazia.

**`destroi_lista()`**
- **Descrição**: A função libera toda a memória alocada para uma lista ligada, representada pela estrutura `t_lista`, e redefine os parâmetros da lista para indicar que ela está vazia.
- **Lógica**: 
    1. **Verificação se a Lista está Vazia**:
        - A função verifica se a lista está vazia utilizando `lista_vazia`.
        - Se a lista estiver vazia, a função retorna imediatamente, pois não há nada para destruir.
    2. **Liberação da Memória dos Nós**:
        - Inicializa dois ponteiros: `aux` para o nó atual e `aux_2` para o próximo nó.
        - Usa um loop para percorrer a lista:
            - Dentro do loop, libera a memória do nó apontado por `aux`.
            - Atualiza `aux` para `aux_2` e `aux_2` para o próximo nó (`aux_2->next`).
        - Após o loop, libera a memória do último nó.
    4. **Redefinição dos Parâmetros da Lista**:
        - Define o tamanho da lista (`tamanho`) como 0.
        - Define o ponteiro inicial (`ini`) como `NULL`, indicando que a lista está vazia.
- **Parâmetros**: 
    - `t_lista *l`: Um ponteiro para a estrutura t_lista que representa a lista a ser destruída.
- **Retorno**: A função não retorna um valor. Ela modifica diretamente a estrutura da lista para liberar a memória e redefinir seus parâmetros.

**`copia_lista()`**
- **Descrição**: A função copia todos os elementos de uma lista ligada `lista1` para outra lista ligada `lista2`. Se `lista1` estiver vazia, a função retorna imediatamente sem fazer alterações.
- **Lógica**: 
    1. **Verificação se a Lista está Vazia**:
        - A função verifica se `lista1` está vazia utilizando `lista_vazia`.
        - Se `lista1` estiver vazia, retorna `0`, indicando que não há elementos para copiar.
    2. **Inicialização e Inserção de Elementos**:
        - Inicializa um ponteiro auxiliar `aux_l` para percorrer `lista1`, começando no início da lista (`lista1->ini`).
        - Insere o valor do primeiro nó de `lista1` no início de `lista2` usando `insere_inicio_lista`.
        - Usa um loop para percorrer `lista1` a partir do segundo nó:
            - Para cada nó, insere o valor no final de `lista2` usando `insere_fim_lista`.
    3. **Atualização do Tamanho da Lista**: Define o tamanho de `lista2` igual ao tamanho de `lista1`.
- **Parâmetros**: 
    - `t_lista *lista1`: Um ponteiro para a lista ligada que será copiada.
    - `t_lista *lista2`: Um ponteiro para a lista ligada onde os elementos serão copiados.
- **Retorno**: Retorna `1` se a cópia for realizada com sucesso. `0` se `lista1` estiver vazia e não houver elementos para copiar.
