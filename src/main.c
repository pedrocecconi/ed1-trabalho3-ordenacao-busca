/* ========================================================================== */
/* UNIVERSIDADE FEDERAL DO PARANA - UFPR                                      */
/* SETOR DE EDUCACAO PROFISSIONAL E TECNOLOGICA - SEPT                        */
/* TECNOLOGIA EM ANALISE E DESENVOLVIMENTO DE SISTEMAS                        */
/* ESTRUTURA DE DADOS I - PROF. ME. ANDREIA DE JESUS                          */
/* 3o TRABALHO PRATICO - METODOS DE ORDENACAO E BUSCA (MENU PRINCIPAL)        */
/* ========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/ordenacao.h"
#include "../include/busca.h"
#include "../include/vetores.h"

#define QTD_TAMANHOS 6
#define QTD_METODOS  4
#define QTD_TIPOS    3

static const int TAMANHOS[QTD_TAMANHOS] = {10, 100, 1000, 10000, 100000, 1000000};

/* Ponteiro para funcao: permite percorrer os 4 metodos num laco so */
typedef void (*FuncaoOrdenacao)(int v[], int n, Estatisticas *est);

static const char *NOMES_METODOS[QTD_METODOS] = {
    "Bubblesort", "Selection Sort", "Insertion Sort", "Shell-Sort"
};
static const FuncaoOrdenacao METODOS[QTD_METODOS] = {
    bubbleSort, selectionSort, insertionSort, shellSort
};

/* Texto exigido no enunciado para cada tipo de vetor inicial */
static const char *DESCRICAO_TIPO[QTD_TIPOS] = {
    "inicialmente ordenado",
    "inicialmente ordenado inversamente",
    "com dados posicionados de forma aleatória"
};

/* -------------------------------------------------------------------------- */
/* Utilitarios de leitura do teclado                                          */
/* -------------------------------------------------------------------------- */
static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int lerInteiro(const char *mensagem, int min, int max) {
    int valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1 && valor >= min && valor <= max) {
            limparBuffer();
            return valor;
        }
        limparBuffer();
        printf("Valor invalido! Digite um inteiro entre %d e %d.\n", min, max);
    }
}

static void lerTexto(const char *mensagem, char *destino, int tamanho) {
    size_t len;
    do {
        printf("%s", mensagem);
        if (fgets(destino, tamanho, stdin) == NULL) {
            destino[0] = '\0';
            return;
        }
        len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n')
            destino[--len] = '\0';
    } while (len == 0);
}

/* -------------------------------------------------------------------------- */
/* PARTE 1 - ORDENACAO                                                        */
/* Para cada metodo, para cada tamanho, para cada tipo de vetor:              */
/* aloca -> gera -> ordena (contando e cronometrando) -> grava -> desaloca.   */
/* Total: 4 metodos x 6 tamanhos x 3 tipos = 72 ordenacoes (18 por metodo).   */
/* -------------------------------------------------------------------------- */
static void executarOrdenacao(void) {
    char nomeArquivo[256], tamanhoFmt[32], compFmt[32], permFmt[32];
    int m, t, k, n, limite, maxTamanhos;
    int *v;
    double inicio, fim, tempo, tempoTotal;
    Estatisticas est;
    FILE *arq;

    lerTexto("\nNome do arquivo de saida (ex.: resultado.txt): ", nomeArquivo,
             (int) sizeof(nomeArquivo) - 5);
    if (strstr(nomeArquivo, ".txt") == NULL)
        strcat(nomeArquivo, ".txt");

    printf("\nAte qual tamanho de conjunto?\n");
    printf("1. Todos (10 a 1.000.000) - versao completa pedida no trabalho\n");
    printf("   ATENCAO: os metodos O(n^2) com 1.000.000 de elementos demoram\n");
    printf("   bastante (uns 35 minutos no total).\n");
    printf("2. Ate 100.000 (roda em menos de 1 minuto, bom para demonstrar)\n");
    limite = lerInteiro("Opcao: ", 1, 2);
    maxTamanhos = (limite == 1) ? QTD_TAMANHOS : QTD_TAMANHOS - 1;

    arq = fopen(nomeArquivo, "w");
    if (arq == NULL) {
        printf("Erro: nao foi possivel criar o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    printf("\nGravando resultados em '%s'...\n", nomeArquivo);
    tempoTotal = tempoAtual();

    for (m = 0; m < QTD_METODOS; m++) {
        fprintf(arq, "Método de ordenação: %s\n\n", NOMES_METODOS[m]);
        printf("\n=== %s ===\n", NOMES_METODOS[m]);

        for (t = 0; t < maxTamanhos; t++) {
            n = TAMANHOS[t];
            formatarMilhar(n, tamanhoFmt);

            for (k = 0; k < QTD_TIPOS; k++) {
                v = criarVetor(n, (TipoVetor) k);       /* aloca e gera */
                if (v == NULL) {
                    printf("Erro: memoria insuficiente para %s elementos.\n", tamanhoFmt);
                    fclose(arq);
                    return;
                }

                est.comparacoes = 0;
                est.permutacoes = 0;

                inicio = tempoAtual();
                METODOS[m](v, n, &est);                 /* ordena */
                fim = tempoAtual();
                tempo = fim - inicio;

                formatarMilhar(est.comparacoes, compFmt);
                formatarMilhar(est.permutacoes, permFmt);

                fprintf(arq, "Conjunto com %s elementos – %s\n", tamanhoFmt, DESCRICAO_TIPO[k]);
                fprintf(arq, "Quantidade de Comparações: %s\n", compFmt);
                fprintf(arq, "Quantidades de Permutações: %s\n", permFmt);
                fprintf(arq, "Tempo gasto para ordenação: %.9f segundos\n\n", tempo);
                fflush(arq);   /* grava na hora: se fechar o programa no meio, nao perde o que ja rodou */

                printf("%9s elementos | %-10s | comp: %19s | perm: %19s | %12.6f s | %s\n",
                       tamanhoFmt,
                       k == ORDENADO ? "ordenado" : (k == INVERSO ? "inverso" : "aleatorio"),
                       compFmt, permFmt, tempo,
                       estaOrdenado(v, n) ? "OK" : "ERRO");

                free(v);                                /* desaloca */
            }
        }
        fprintf(arq, "\n");
    }

    fclose(arq);
    printf("\nConcluido em %.2f segundos. Resultados em '%s'.\n",
           tempoAtual() - tempoTotal, nomeArquivo);
}

/* -------------------------------------------------------------------------- */
/* PARTE 2 - BUSCA                                                            */
/* -------------------------------------------------------------------------- */
static void imprimirVetor(const char *titulo, const int v[], int n) {
    int i;
    printf("%s\n", titulo);
    for (i = 0; i < n; i++) {
        printf("%6d", v[i]);
        if ((i + 1) % 10 == 0 || i == n - 1)
            printf("\n");
    }
}

static void executarBusca(void) {
    int n, i;
    int chaves[3];
    int *original, *ordenado;
    long long compSeq[3], compBin[3];
    int posSeq[3], posBin[3];

    n = lerInteiro("\nQuantidade de elementos do conjunto: ", 1, 100000000);

    original = (int *) malloc((size_t) n * sizeof(int));
    ordenado = (int *) malloc((size_t) n * sizeof(int));
    if (original == NULL || ordenado == NULL) {
        printf("Erro: memoria insuficiente.\n");
        free(original);
        free(ordenado);
        return;
    }

    /* Numeros aleatorios entre 100 e 100.000 */
    for (i = 0; i < n; i++)
        original[i] = aleatorioEntre(100, 100000);

    /* Copia e ordena a copia (Shell-Sort) para a busca binaria.
       O vetor original continua desordenado para a busca sequencial. */
    memcpy(ordenado, original, (size_t) n * sizeof(int));
    shellSort(ordenado, n, NULL);

    if (n <= 100) {
        imprimirVetor("\nVetor original (nao ordenado):", original, n);
        imprimirVetor("\nVetor ordenado:", ordenado, n);
    } else {
        printf("\nVetor gerado com %d elementos (grande demais para mostrar na tela).\n", n);
    }

    printf("\nInforme 3 numeros inteiros para buscar:\n");
    for (i = 0; i < 3; i++) {
        char msg[40];
        sprintf(msg, "Numero %d: ", i + 1);
        chaves[i] = lerInteiro(msg, -2147483647, 2147483647);
    }

    for (i = 0; i < 3; i++) {
        posSeq[i] = buscaSequencial(original, n, chaves[i], &compSeq[i]);
        posBin[i] = buscaBinaria(ordenado, n, chaves[i], &compBin[i]);
    }

    printf("\n================ RESULTADO DAS BUSCAS ================\n");
    for (i = 0; i < 3; i++) {
        printf("\nNumero %d: %d\n", i + 1, chaves[i]);
        if (posSeq[i] != -1)
            printf("  Encontra-se no conjunto de dados.\n");
        else
            printf("  NAO se encontra no conjunto de dados.\n");

        printf("  Pesquisa Sequencial (vetor original): %lld comparacoes", compSeq[i]);
        if (posSeq[i] != -1) printf(" (posicao %d)", posSeq[i]);
        printf("\n");

        printf("  Pesquisa Binaria    (vetor ordenado): %lld comparacoes", compBin[i]);
        if (posBin[i] != -1) printf(" (posicao %d)", posBin[i]);
        printf("\n");
    }
    printf("\n======================================================\n");

    free(original);
    free(ordenado);
}

/* -------------------------------------------------------------------------- */
int main(void) {
    int opcao;

    srand((unsigned) time(NULL));

    do {
        printf("\n======================================================\n");
        printf("  3o TRABALHO PRATICO: ORDENACAO E BUSCA (ED I - UFPR)\n");
        printf("======================================================\n");
        printf("1. Ordenacao de Dados (gera o arquivo .txt)\n");
        printf("2. Busca de Dados (sequencial e binaria)\n");
        printf("3. Sair\n");
        printf("------------------------------------------------------\n");
        opcao = lerInteiro("Selecione uma opcao: ", 1, 3);

        switch (opcao) {
            case 1: executarOrdenacao(); break;
            case 2: executarBusca();     break;
            case 3: printf("\nPrograma encerrado.\n"); break;
        }
    } while (opcao != 3);

    return 0;
}
