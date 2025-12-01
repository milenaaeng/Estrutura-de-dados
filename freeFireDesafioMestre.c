/*
  Sistema de Prioritização e Montagem de Componentes
  - Implementa Bubble Sort (por nome), Insertion Sort (por tipo),
    Selection Sort (por prioridade).
  - Mede comparações e tempo (clock()).
  - Busca binária por nome (aplicável apenas após ordenação por nome).
  - Entrada de até 20 componentes usando fgets().
  - Interface por menu.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define NAME_LEN 30
#define TYPE_LEN 20

typedef struct {
    char nome[NAME_LEN];
    char tipo[TYPE_LEN];
    int prioridade; // 1 a 10
} Componente;

/* ---------- Funções utilitárias ---------- */

// Remove '\n' vindo de fgets, se houver
void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

// Lê string segura usando fgets e remove newline
void ler_string(char *buffer, int tamanho, const char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, tamanho, stdin) != NULL) {
        strip_newline(buffer);
    } else {
        // Em caso de erro, garante string vazia
        buffer[0] = '\0';
    }
}

// Lê inteiro seguro usando fgets + sscanf
int ler_inteiro(const char *prompt) {
    char buf[64];
    int val;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (sscanf(buf, "%d", &val) == 1) return val;
        printf("Entrada inválida. Tente novamente.\n");
    }
}

/* ---------- Exibição ---------- */

void mostrarComponentes(Componente arr[], int n) {
    printf("\n--- Componentes (%d) ---\n", n);
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("%-3s %-30s %-15s %-10s\n", "#", "NOME", "TIPO", "PRIORIDADE");
    for (int i = 0; i < n; i++) {
        printf("%-3d %-30s %-15s %-10d\n",
               i+1, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
}

/* ---------- Algoritmos de ordenação ---------- */

/*
  Bubble Sort por nome (string)
  - compara nomes com strcmp
  - conta comparações (incrementa *comparacoes)
  - faz troca de structs completas
*/
void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; // otimização: se já está ordenado
    }
}

/*
  Insertion Sort por tipo (string)
  - compara tipos com strcmp
  - conta comparações em *comparacoes
*/
void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = arr[i];
        int j = i - 1;
        // mover enquanto tipo[j] > chave.tipo
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = chave;
    }
}

/*
  Selection Sort por prioridade (int)
  - Ordena em ordem crescente de prioridade (1 = mais urgente)
  - Conta comparações em *comparacoes (comparação entre prioridades)
*/
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[menor].prioridade) {
                menor = j;
            }
        }
        if (menor != i) {
            Componente tmp = arr[i];
            arr[i] = arr[menor];
            arr[menor] = tmp;
        }
    }
}

/* ---------- Busca binária (aplicável após ordenação por nome) ---------- */
/*
  retorna índice do componente encontrado ou -1 se não encontrado
  também registra número de comparações em *comparacoes
  usa strcmp para comparar strings
*/
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes) {
    int esquerda = 0, direita = n - 1;
    *comparacoes = 0;
    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, arr[meio].nome);
        if (cmp == 0) return meio;
        else if (cmp > 0) esquerda = meio + 1;
        else direita = meio - 1;
    }
    return -1;
}

/* ---------- Medição de tempo genérica ---------- */
/*
  Executa a rotina passada (que deve ser do tipo void func(Componente[], int, long *))
  e mede tempo gasto em segundos, além de capturar comparações retornadas através do ponteiro compPtr.
*/
typedef void (*SortFunc)(Componente[], int, long *);

double medirTempoOrdenacao(SortFunc func, Componente arr[], int n, long *compPtr) {
    clock_t inicio = clock();
    func(arr, n, compPtr);
    clock_t fim = clock();
    double segundos = ((double)(fim - inicio)) / (double)CLOCKS_PER_SEC;
    return segundos;
}

/* ---------- Menu e fluxo principal ---------- */

int main() {
    Componente componentes[MAX_COMPONENTES];
    int total = 0;
    int opcao;
    int ordenadoPorNome = 0; // flag para indicar se vetor está ordenado por nome

    printf("=== MONTAGEM DA TORRE DE RESGATE - MÓDULO AVANÇADO ===\n");

    while (1) {
        printf("\nMenu principal:\n");
        printf("1 - Cadastrar componente\n");
        printf("2 - Mostrar componentes\n");
        printf("3 - Ordenar com Bubble Sort (por NOME)\n");
        printf("4 - Ordenar com Insertion Sort (por TIPO)\n");
        printf("5 - Ordenar com Selection Sort (por PRIORIDADE)\n");
        printf("6 - Buscar componente-chave (BUSCA BINÁRIA por NOME) [requer ordenação por nome]\n");
        printf("7 - Zerar cadastro (remover todos)\n");
        printf("0 - Sair\n");
        opcao = ler_inteiro("Escolha uma opção: ");

        if (opcao == 0) {
            printf("Encerrando sistema.\n");
            break;
        }

        if (opcao == 1) {
            if (total >= MAX_COMPONENTES) {
                printf("Capacidade máxima (%d) atingida.\n", MAX_COMPONENTES);
                continue;
            }
            Componente c;
            ler_string(c.nome, NAME_LEN, "Digite o NOME do componente: ");
            ler_string(c.tipo, TYPE_LEN, "Digite o TIPO do componente: ");
            int p;
            while (1) {
                p = ler_inteiro("Digite a PRIORIDADE (1-10, 1 = maior prioridade): ");
                if (p >= 1 && p <= 10) break;
                printf("Prioridade inválida. Informe entre 1 e 10.\n");
            }
            c.prioridade = p;
            componentes[total++] = c;
            ordenadoPorNome = 0; // inserção quebra a ordenação por nome
            printf("Componente cadastrado com sucesso.\n");
            mostrarComponentes(componentes, total);
        }
        else if (opcao == 2) {
            mostrarComponentes(componentes, total);
        }
        else if (opcao == 3) { // Bubble Sort por nome
            if (total == 0) {
                printf("Nenhum componente para ordenar.\n");
                continue;
            }
            // vamos copiar o vetor para preservar possível comparação? não necessário; operamos diretamente.
            long comparacoes = 0;
            double tempo = medirTempoOrdenacao(bubbleSortNome, componentes, total, &comparacoes);
            ordenadoPorNome = 1;
            printf("\nBubble Sort concluído (por NOME).\nComparações: %ld\nTempo: %.6f segundos\n", comparacoes, tempo);
            mostrarComponentes(componentes, total);
        }
        else if (opcao == 4) { // Insertion Sort por tipo
            if (total == 0) {
                printf("Nenhum componente para ordenar.\n");
                continue;
            }
            long comparacoes = 0;
            double tempo = medirTempoOrdenacao(insertionSortTipo, componentes, total, &comparacoes);
            ordenadoPorNome = 0; // ordenação por tipo quebra ordenação por nome
            printf("\nInsertion Sort concluído (por TIPO).\nComparações: %ld\nTempo: %.6f segundos\n", comparacoes, tempo);
            mostrarComponentes(componentes, total);
        }
        else if (opcao == 5) { // Selection Sort por prioridade
            if (total == 0) {
                printf("Nenhum componente para ordenar.\n");
                continue;
            }
            long comparacoes = 0;
            double tempo = medirTempoOrdenacao(selectionSortPrioridade, componentes, total, &comparacoes);
            ordenadoPorNome = 0; // ordenação por prioridade quebra ordenação por nome
            printf("\nSelection Sort concluído (por PRIORIDADE).\nComparações: %ld\nTempo: %.6f segundos\n", comparacoes, tempo);
            mostrarComponentes(componentes, total);
        }
        else if (opcao == 6) {
            if (!ordenadoPorNome) {
                printf("Busca binária requer que os componentes estejam ordenados por NOME (use opção 3 primeiro).\n");
                continue;
            }
            if (total == 0) {
                printf("Nenhum componente cadastrado.\n");
                continue;
            }
            char chave[NAME_LEN];
            ler_string(chave, NAME_LEN, "Digite o NOME do componente-chave a buscar: ");
            long comparacoes = 0;
            clock_t t0 = clock();
            int pos = buscaBinariaPorNome(componentes, total, chave, &comparacoes);
            clock_t t1 = clock();
            double tempo = ((double)(t1 - t0)) / CLOCKS_PER_SEC;
            if (pos >= 0) {
                printf("\nComponente encontrado na posição %d (índice %d).\n", pos+1, pos);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
            } else {
                printf("\nComponente NÃO encontrado.\n");
            }
            printf("Comparações na busca binária: %ld\nTempo da busca: %.6f segundos\n", comparacoes, tempo);
        }
        else if (opcao == 7) {
            total = 0;
            ordenadoPorNome = 0;
            printf("Cadastro zerado.\n");
        }
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
