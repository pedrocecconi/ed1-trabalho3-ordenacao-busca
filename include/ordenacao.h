/* ========================================================================== */
/* UNIVERSIDADE FEDERAL DO PARANA - UFPR                                      */
/* SETOR DE EDUCACAO PROFISSIONAL E TECNOLOGICA - SEPT                        */
/* TECNOLOGIA EM ANALISE E DESENVOLVIMENTO DE SISTEMAS                        */
/* ESTRUTURA DE DADOS I - 3o TRABALHO PRATICO                                 */
/* METODOS DE ORDENACAO (Bubble, Selection, Insertion e Shell Sort)           */
/* ========================================================================== */

#ifndef ORDENACAO_H
#define ORDENACAO_H

/* Contadores de cada ordenacao.
   long long porque com 1.000.000 de elementos os metodos O(n^2)
   passam de 500 bilhoes de comparacoes (nao cabe em int). */
typedef struct {
    long long comparacoes;
    long long permutacoes;
} Estatisticas;

/* Todas recebem o vetor, o tamanho e um ponteiro para os contadores.
   Se 'est' for NULL a funcao ordena sem contar (usado na parte de busca). */
void bubbleSort   (int v[], int n, Estatisticas *est);
void selectionSort(int v[], int n, Estatisticas *est);
void insertionSort(int v[], int n, Estatisticas *est);
void shellSort    (int v[], int n, Estatisticas *est);

/* Retorna 1 se o vetor esta em ordem crescente, 0 caso contrario */
int estaOrdenado(const int v[], int n);

#endif /* ORDENACAO_H */
