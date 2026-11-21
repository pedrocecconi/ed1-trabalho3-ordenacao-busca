/* ========================================================================== */
/* ESTRUTURA DE DADOS I - 3o TRABALHO PRATICO                                 */
/* GERACAO DE VETORES E MEDICAO DE TEMPO                                      */
/* ========================================================================== */

#include "../include/vetores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

/* -------------------------------------------------------------------------- */
/* Junta dois rand() para ter um numero grande o bastante. No Windows         */
/* RAND_MAX = 32767, e so com rand() nao da pra sortear posicao num vetor     */
/* de 1.000.000 de elementos.                                                 */
/* -------------------------------------------------------------------------- */
static unsigned long aleatorioGrande(void) {
    return ((unsigned long)(rand() & 0x7FFF) << 15) | (unsigned long)(rand() & 0x7FFF);
}

int aleatorioEntre(int min, int max) {
    return min + (int)(aleatorioGrande() % (unsigned long)(max - min + 1));
}

/* -------------------------------------------------------------------------- */
/* Gera os valores 1, 2, ..., n (assim nunca ha repetidos) e arruma:          */
/*   ORDENADO  -> 1 2 3 ... n                                                 */
/*   INVERSO   -> n ... 3 2 1                                                 */
/*   ALEATORIO -> 1..n embaralhado (algoritmo de Fisher-Yates)                */
/* -------------------------------------------------------------------------- */
int *criarVetor(int n, TipoVetor tipo) {
    int i, j, aux;
    int *v = (int *) malloc((size_t) n * sizeof(int));

    if (v == NULL) return NULL;

    for (i = 0; i < n; i++) {
        if (tipo == INVERSO)
            v[i] = n - i;
        else
            v[i] = i + 1;
    }

    if (tipo == ALEATORIO) {
        for (i = n - 1; i > 0; i--) {
            j = aleatorioEntre(0, i);
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
    }
    return v;
}

/* -------------------------------------------------------------------------- */
double tempoAtual(void) {
#ifdef _WIN32
    LARGE_INTEGER freq, contador;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&contador);
    return (double) contador.QuadPart / (double) freq.QuadPart;
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double) t.tv_sec + (double) t.tv_nsec / 1e9;
#endif
}

/* -------------------------------------------------------------------------- */
void formatarMilhar(long long n, char *saida) {
    char digitos[32];
    int len, i, k = 0;

    sprintf(digitos, "%lld", n);
    len = (int) strlen(digitos);
    for (i = 0; i < len; i++) {
        saida[k++] = digitos[i];
        if ((len - i - 1) > 0 && (len - i - 1) % 3 == 0)
            saida[k++] = '.';
    }
    saida[k] = '\0';
}
