/* ========================================================================== */
/* ESTRUTURA DE DADOS I - 3o TRABALHO PRATICO                                 */
/* IMPLEMENTACAO DOS METODOS DE BUSCA                                         */
/* ========================================================================== */

#include "../include/busca.h"

/* -------------------------------------------------------------------------- */
/* PESQUISA SEQUENCIAL                                                        */
/* Olha posicao por posicao, do inicio ao fim.                                */
/* Cada teste v[i] == chave conta 1 comparacao.                               */
/* Achou na posicao 0: 1 comparacao. Nao achou: n comparacoes.                */
/* -------------------------------------------------------------------------- */
int buscaSequencial(const int v[], int n, int chave, long long *comparacoes) {
    int i;

    *comparacoes = 0;
    for (i = 0; i < n; i++) {
        (*comparacoes)++;
        if (v[i] == chave)
            return i;
    }
    return -1;
}

/* -------------------------------------------------------------------------- */
/* PESQUISA BINARIA (iterativa, como pede o material da aula)                 */
/* Compara a chave com o elemento do meio. Se for igual, achou. Se a chave    */
/* for menor, descarta a metade direita; se for maior, descarta a esquerda.   */
/* Cada vez que a chave e comparada com o elemento do meio conta 1 comparacao */
/* (uma por iteracao). Pior caso: cerca de log2(n) + 1 comparacoes.           */
/* -------------------------------------------------------------------------- */
int buscaBinaria(const int v[], int n, int chave, long long *comparacoes) {
    int inicio = 0, fim = n - 1, meio;

    *comparacoes = 0;
    while (inicio <= fim) {
        meio = inicio + (fim - inicio) / 2;   /* evita estouro de int */
        (*comparacoes)++;
        if (v[meio] == chave)
            return meio;
        else if (chave < v[meio])
            fim = meio - 1;
        else
            inicio = meio + 1;
    }
    return -1;
}
