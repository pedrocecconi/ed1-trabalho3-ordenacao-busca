# 3º trabalho prático: métodos de ordenação e busca

Estrutura de Dados I, TADS/UFPR, Prof. Andreia de Jesus. Entrega no Moodle até 18h30 de 24/11/2026; defesas em 24/11 e 01/12.

## Estrutura do projeto

```text
trabalho3/
├── include/
│   ├── ordenacao.h   # struct Estatisticas e protótipos dos 4 métodos
│   ├── busca.h       # protótipos da pesquisa sequencial e binária
│   └── vetores.h     # geração dos vetores, relógio e formatação
├── src/
│   ├── ordenacao.c   # Bubblesort, Selection, Insertion e Shell-Sort
│   ├── busca.c       # Pesquisa Sequencial e Pesquisa Binária
│   ├── vetores.c     # vetor ordenado/inverso/aleatório, tempo
│   └── main.c        # menu, parte 1 (arquivo .txt) e parte 2 (busca)
└── README.md
```

## Como compilar

No terminal do VS Code, dentro da pasta do projeto:

```bash
gcc -Wall -Wextra -O2 -Iinclude src/main.c src/ordenacao.c src/busca.c src/vetores.c -o trabalho3.exe
./trabalho3.exe
```

Compile sempre com `-O2`: ele deixa os métodos O(n²) bem mais rápidos com 1.000.000 de elementos.

## Parte 1: ordenação

O programa pergunta o nome do arquivo (a professora informa na defesa; se não digitar `.txt`, ele acrescenta). Depois roda 4 métodos × 6 tamanhos × 3 tipos de vetor = 72 ordenações, 18 por método. Para cada uma: aloca o vetor com `malloc`, gera os dados, ordena contando e cronometrando, grava no arquivo e dá `free`.

Os vetores guardam os valores 1, 2, ..., n, então nunca há repetidos. O ordenado é 1..n, o inverso é n..1 e o aleatório é 1..n embaralhado com Fisher-Yates.

Cada resultado é gravado no arquivo assim que termina (`fflush`), então se o programa for fechado no meio, o que já rodou fica salvo. Na tela aparece uma linha por ordenação com "OK" se o vetor saiu ordenado de verdade (função `estaOrdenado`).

### O que conta como comparação e permutação

Comparação é cada vez que dois elementos do vetor são comparados (`v[j] > v[j+1]`). O teste do `for` com índices não conta.

Permutação depende do método. No Bubblesort e no Selection Sort é cada troca entre duas posições. No Insertion e no Shell-Sort não existe troca: o elemento é guardado em `aux` e os maiores são deslocados uma casa (`v[j] = v[j-h]`). Cada deslocamento conta 1.

Os contadores são `long long`, porque com 1.000.000 de elementos os métodos O(n²) chegam a 499.999.500.000 comparações, e `int` vai só até 2.147.483.647.

### Valores esperados (dá pra conferir na defesa)

Com n elementos:

| Método | Ordenado | Inverso | Aleatório |
|---|---|---|---|
| Bubblesort | n−1 comp., 0 perm. | n(n−1)/2 comp. e perm. | perto de n(n−1)/2 comp., ~n²/4 perm. |
| Selection | n(n−1)/2 comp., 0 perm. | n(n−1)/2 comp., n/2 perm. | n(n−1)/2 comp., quase n perm. |
| Insertion | n−1 comp., 0 perm. | n(n−1)/2 comp. e perm. | ~n²/4 comp. e perm. |
| Shell-Sort | bem menos que os outros em todos os casos | | |

Com n = 100.000, por exemplo, n(n−1)/2 = 4.999.950.000. O Bubblesort usa a melhoria da flag `trocou` (o material pergunta "Como melhorar?"): se uma passada inteira não troca nada, ele para. Por isso no vetor ordenado ele faz só n−1 comparações.

O Selection faz sempre n(n−1)/2 comparações, qualquer que seja a entrada. No inverso ele faz só n/2 trocas porque cada troca coloca dois elementos no lugar certo de uma vez. Ele só troca quando o menor não está já na posição `i`. Durante a procura, o valor do menor fica guardado em `valorMenor`, pra não reler `v[menor]` a cada comparação; isso deixou o método 8 vezes mais rápido com `-O2`.

O Shell-Sort usa a sequência de Knuth do material da aula: 1, 4, 13, 40, 121, 364, 1093... O h inicial é o primeiro termo maior que n, e a cada passada h = h/3.

### Tempo

O tempo é medido com `QueryPerformanceCounter` no Windows (precisão abaixo de 1 microssegundo). O `clock()` do Windows mede só milissegundos e mostraria 0 para quase todos os vetores pequenos. Mesmo assim, com 10 elementos o tempo fica na casa dos nanossegundos.

A versão completa (até 1.000.000) demora. Cada método O(n²) com 1 milhão faz cerca de 500 bilhões de comparações. Medindo no seu PC, a estimativa é de uns 35 minutos no total, quase tudo no Bubblesort (só o caso aleatório com 1 milhão leva perto de 20 minutos, porque o processador erra muito a previsão do `if` quando os dados são aleatórios).

Por isso o menu tem duas opções: a completa (a que o trabalho pede) e uma que vai até 100.000 e terminou em 22 segundos no seu PC, pra demonstrar na hora. Vale rodar a completa antes, com calma, e levar o arquivo pronto.

## Parte 2: busca

O usuário informa quantos elementos quer. O programa sorteia números entre 100 e 100.000 (aqui pode repetir, o enunciado não proíbe, e com mais de 99.901 elementos nem daria pra não repetir).

Depois faz uma cópia do vetor e ordena a cópia com o Shell-Sort. A Pesquisa Sequencial roda no vetor original desordenado e a Pesquisa Binária roda na cópia ordenada, como pede a observação do enunciado.

O usuário digita 3 números. Para cada um o programa mostra se encontra-se ou não no conjunto e quantas comparações cada pesquisa fez. Se o vetor tiver até 100 elementos, ele também é mostrado na tela (original e ordenado), o que ajuda a provar o resultado na defesa.

Na Pesquisa Sequencial cada `v[i] == chave` conta 1. Achou na posição 0 é 1 comparação; não achou é n comparações.

Na Pesquisa Binária (iterativa, como o material pede) conta 1 comparação por iteração, ou seja, cada vez que a chave é comparada com o elemento do meio. No pior caso são uns log₂(n) + 1: com 99.000 elementos, no máximo 17.

## Perguntas que podem cair na defesa

Por que a binária precisa do vetor ordenado? Porque ela descarta metade do vetor a cada passo supondo que tudo à esquerda do meio é menor e tudo à direita é maior. Num vetor desordenado ela pode dizer que não achou um número que está lá.

Por que o Insertion é tão melhor que o Bubble no aleatório, se os dois são O(n²)? O Insertion faz uns n²/4 comparações (para no lugar certo de cada elemento), enquanto o Bubble faz quase n²/2. E deslocar é mais barato que trocar: uma atribuição contra três.

Por que o Shell-Sort é tão mais rápido? Ele move elementos que estão longe do lugar certo em saltos de h posições. Quando chega no h = 1 (um insertion comum), o vetor já está quase ordenado, e o Insertion num vetor quase ordenado é quase linear.

Por que o Bubble no inverso demora mais que o Selection no inverso, se fazem o mesmo número de comparações? Porque o Bubble faz uma troca em toda comparação (n(n−1)/2 trocas), e o Selection faz só n/2 trocas.
