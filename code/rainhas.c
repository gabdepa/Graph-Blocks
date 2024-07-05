#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>

// Função para verificar se é seguro colocar uma rainha em uma determinada posição 
int is_safe(unsigned int* board, int row, int col, int n, casa* c, unsigned int k) {
    for (unsigned int p = 0; p < k; ++p) {
        if (c[p].linha - 1 == (unsigned int)row && c[p].coluna - 1 == (unsigned int)col) {
            return 0;
        }
    }
    for (int i = 0; i < row; i++) {
        if (board[i] == col || board[i] == (col - (row - i)) || board[i] == (col + (row - i))) {
            return 0;
        }
    }
    return 1;
}

// Função auxiliar para resolver o problema das N-rainhas com backtracking
int solve_n_queens_bt(unsigned int* board, int row, int n, casa* c, unsigned int k) {
    if (row >= n) {
        return 1;
    }
    for (unsigned int i = 0; i < n; i++) {
        if (is_safe(board, row, i, n, c, k)) {
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

    if (solve_n_queens_bt(board, 0, (int)n, c, k)) {
        for (unsigned int i = 0; i < n; i++) {
            r[i] = board[i] + 1;  // Adiciona 1 ao índice da coluna
        }
    }

    free(board);
    return r;
}

// Função para resolver o problema das N-rainhas com conjuntos independentes
int solve_n_queens_ci(unsigned int *board, int row, int n, casa *c, unsigned int k)
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
        for (int j = 0; j < row; j++)
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

    if (solve_n_queens_ci(board, 0, (int)n, c, k))
    {
        for (unsigned int i = 0; i < n; i++)
        {
            r[i] = board[i] + 1; // Adiciona 1 ao índice da coluna
        }
    }

    free(board);
    return r;
}
