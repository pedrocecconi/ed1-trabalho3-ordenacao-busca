/* ========================================================================== */
/* ESTRUTURA DE DADOS I - 3o TRABALHO PRATICO                                 */
/* METODOS DE BUSCA (Pesquisa Sequencial e Pesquisa Binaria)                  */
/* ========================================================================== */

#ifndef BUSCA_H
#define BUSCA_H

/* Ambas retornam o indice do elemento ou -1 se nao encontrar.
   O numero de comparacoes feitas e devolvido em *comparacoes. */

/* Funciona em qualquer vetor (ordenado ou nao) */
int buscaSequencial(const int v[], int n, int chave, long long *comparacoes);

/* SO funciona em vetor ordenado em ordem crescente */
int buscaBinaria(const int v[], int n, int chave, long long *comparacoes);

#endif /* BUSCA_H */
