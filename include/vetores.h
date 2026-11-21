/* ========================================================================== */
/* ESTRUTURA DE DADOS I - 3o TRABALHO PRATICO                                 */
/* GERACAO DE VETORES E MEDICAO DE TEMPO                                      */
/* ========================================================================== */

#ifndef VETORES_H
#define VETORES_H

/* Tipo de vetor inicial (antes da ordenacao) */
typedef enum {
    ORDENADO = 0,
    INVERSO = 1,
    ALEATORIO = 2
} TipoVetor;

/* Aloca dinamicamente um vetor com n inteiros NAO repetidos (1..n),
   arrumados conforme o tipo. Retorna NULL se faltar memoria. */
int *criarVetor(int n, TipoVetor tipo);

/* Inteiro aleatorio em [min, max]. Funciona mesmo no Windows,
   onde RAND_MAX e so 32767. */
int aleatorioEntre(int min, int max);

/* Relogio de alta precisao em segundos (QueryPerformanceCounter no
   Windows, clock_gettime no Linux). clock() no Windows so mede
   milissegundos e daria 0 para os vetores pequenos. */
double tempoAtual(void);

/* Escreve n com separador de milhar: 1000000 -> "1.000.000" */
void formatarMilhar(long long n, char *saida);

#endif /* VETORES_H */
