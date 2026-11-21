/* ========================================================================== */
/* ESTRUTURA DE DADOS I - 3o TRABALHO PRATICO                                 */
/* IMPLEMENTACAO DOS METODOS DE ORDENACAO                                     */
/* ========================================================================== */
/*                                                                            */
/* O que e contado em cada metodo:                                            */
/*  - Comparacao: cada vez que dois ELEMENTOS DO VETOR sao comparados         */
/*    (ex.: v[j] > v[j+1]). Comparacoes de indices do for nao entram.         */
/*  - Permutacao: cada vez que um elemento muda de posicao no vetor.          */
/*      Bubble e Selection: cada troca entre duas posicoes conta 1.           */
/*      Insertion e Shell: cada deslocamento v[j] = v[j-h] conta 1            */
/*      (esses metodos nao fazem troca, eles "empurram" os maiores).          */
/* ========================================================================== */

#include "../include/ordenacao.h"
#include <stddef.h>

/* Os contadores ficam em variaveis locais (comp e perm) e so sao copiados
   para 'est' no fim de cada metodo. Assim o codigo de cada metodo fica
   limpo (comp++ e perm++) e o caso est == NULL e tratado num lugar so. */
static void salvar(Estatisticas *est, long long comp, long long perm) {
    if (est != NULL) {
        est->comparacoes = comp;
        est->permutacoes = perm;
    }
}

static void trocar(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

/* -------------------------------------------------------------------------- */
/* BUBBLE SORT (com a melhoria da flag 'trocou')                              */
/* Compara vizinhos e troca se estiverem fora de ordem. A cada passada o      */
/* maior elemento que sobrou "sobe" para o fim. Se uma passada inteira nao    */
/* trocar nada, o vetor ja esta ordenado e o metodo para.                     */
/* Melhor caso (ordenado): n-1 comparacoes, 0 trocas.                         */
/* Pior caso (inverso): n(n-1)/2 comparacoes e n(n-1)/2 trocas.               */
/* -------------------------------------------------------------------------- */
void bubbleSort(int v[], int n, Estatisticas *est) {
    long long comp = 0, perm = 0;
    int i, j, trocou;

    for (i = 0; i < n - 1; i++) {
        trocou = 0;
        for (j = 0; j < n - 1 - i; j++) {
            comp++;
            if (v[j] > v[j + 1]) {
                trocar(&v[j], &v[j + 1]);
                perm++;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    salvar(est, comp, perm);
}

/* -------------------------------------------------------------------------- */
/* SELECTION SORT                                                             */
/* Procura o menor elemento da parte nao ordenada e coloca na posicao i.      */
/* Comparacoes: sempre n(n-1)/2, nao importa a entrada.                       */
/* Trocas: no maximo n-1 (so troca quando o menor nao ja esta no lugar).      */
/* -------------------------------------------------------------------------- */
void selectionSort(int v[], int n, Estatisticas *est) {
    long long comp = 0, perm = 0;
    int i, j, menor, valorMenor;

    for (i = 0; i < n - 1; i++) {
        menor = i;
        valorMenor = v[i];   /* guarda o valor do menor pra nao reler v[menor] toda hora */
        for (j = i + 1; j < n; j++) {
            comp++;
            if (v[j] < valorMenor) {
                menor = j;
                valorMenor = v[j];
            }
        }
        if (menor != i) {
            trocar(&v[i], &v[menor]);
            perm++;
        }
    }
    salvar(est, comp, perm);
}

/* -------------------------------------------------------------------------- */
/* INSERTION SORT                                                             */
/* Pega o elemento v[i] e vai deslocando para a direita os maiores que ele    */
/* na parte ja ordenada, ate achar o lugar dele (igual organizar cartas).     */
/* Melhor caso (ordenado): n-1 comparacoes, 0 deslocamentos.                  */
/* Pior caso (inverso): n(n-1)/2 comparacoes e n(n-1)/2 deslocamentos.        */
/* -------------------------------------------------------------------------- */
void insertionSort(int v[], int n, Estatisticas *est) {
    long long comp = 0, perm = 0;
    int i, j, aux;

    for (i = 1; i < n; i++) {
        aux = v[i];
        j = i - 1;
        while (j >= 0) {
            comp++;
            if (v[j] > aux) {
                v[j + 1] = v[j];
                perm++;
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = aux;
    }
    salvar(est, comp, perm);
}

/* -------------------------------------------------------------------------- */
/* SHELL SORT (sequencia de Knuth: 1, 4, 13, 40, 121, 364, 1093, ...)         */
/* E um insertion sort que compara elementos distantes h posicoes. O h vai    */
/* diminuindo ate 1; na ultima passada (h = 1) ele vira um insertion sort     */
/* comum, mas o vetor ja esta quase ordenado, entao sobra pouco trabalho.     */
/* -------------------------------------------------------------------------- */
void shellSort(int v[], int n, Estatisticas *est) {
    long long comp = 0, perm = 0;
    int i, j, h, aux;

    if (n < 2) {
        salvar(est, 0, 0);
        return;
    }

    /* calcula o h inicial: primeiro valor da sequencia que passa de n */
    h = 1;
    while (h < n)
        h = 3 * h + 1;

    do {
        h = h / 3;   /* volta um termo na sequencia: 13 -> 4 -> 1 */
        for (i = h; i < n; i++) {
            aux = v[i];
            j = i;
            while (j >= h) {
                comp++;
                if (v[j - h] > aux) {
                    v[j] = v[j - h];
                    perm++;
                    j -= h;
                } else {
                    break;
                }
            }
            v[j] = aux;
        }
    } while (h > 1);
    salvar(est, comp, perm);
}

/* -------------------------------------------------------------------------- */
int estaOrdenado(const int v[], int n) {
    int i;
    for (i = 1; i < n; i++)
        if (v[i - 1] > v[i])
            return 0;
    return 1;
}
